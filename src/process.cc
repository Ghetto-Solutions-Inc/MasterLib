/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : process.cc                                                          *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <errno.h>
#include <signal.h>
#include <sys/sysctl.h>
#include <mach-o/dyld.h>

#include "missing/libproc.h"
#include "missing/mach_vm.h"

#include "process.h"


namespace masterlib {

std::shared_ptr<Process> Process::self; // silences compiler, in c++17 can define as inline
Process::Region Process::Region::null{0, 0, 0};

static inline const char *last_path_component(const char *path) {
    bool slash = false;
    long position = strlen(path);

    for (int i = 0; i < position; i++) {
        if (path[i] == '/') slash = true;
    }

    if (slash) {
        while (position >= 0 && path[position] != '/') {
            position--;
        }

        path += position + 1;
    }
    return path;

}

static inline const char *last_path_component(const std::string &str) {
    return last_path_component(str.c_str());
}

__attribute__((constructor))
void Process::ConstructSelfProcess(int argc,
                                   const char **argv,
                                   const char **envp,
                                   const char **apple,
                                   struct ProgramVars *) {
    int pid = getpid();
    std::string name = last_path_component(argv[0]);

    char *path = new char[MAXPATHLEN * 4];
    uint32_t path_size = sizeof(path);

    if (_NSGetExecutablePath(path, &path_size) == -1) {
        delete[] path;
        path = new char[path_size];
        _NSGetExecutablePath(path, &path_size); // cannot fail (?)
    }

    Process::self = Process::Create(pid, name, path);
    delete[] path;
}

std::shared_ptr<Process> Process::FindProcess(pid_t pid) {
    if (pid == getpid()) {
        return Process::self;
    }

    struct proc_bsdinfo proc;

    // this method sometimes requires root, may need to find a backup method
    int res = proc_pidinfo(pid,
                           PROC_PIDTBSDINFO,
                           0,
                           &proc,
                           PROC_PIDTBSDINFO_SIZE);

    if (res != PROC_PIDTBSDINFO_SIZE) {
        return nullptr;
    }
    return Process::Create(proc.pbi_pid,
                           last_path_component(proc.pbi_name),
                           PathForPID(proc.pbi_pid));
}

std::shared_ptr<Process> Process::FindProcess(std::string name) {
    return FindProcess(PIDForName(name));
}

bool Process::MachTask(task_t *out) const {
    if (out) {
        // if this is the current process use mach_task_self
        if (*this == *Process::self) {
            *out = mach_task_self();
            return true;
        } else {
            if (task_for_pid(mach_task_self(), pid_, out) != KERN_SUCCESS) {
                // task_for_pid failed, likely due to lack of entitlements
                return false;
            }
            return true;
        }
    }
    return false;
}

bool Process::Kill() {
    if (kill(pid_, -9) < 0) {
        // check errno
        /*
            [EINVAL]
            The value of the sig argument is an invalid or unsupported signal number.
            [EPERM]
            The process does not have permission to send the signal to any receiving process.
            [ESRCH]
            No process or process group can be found corresponding to that specified by pid.
         */
        switch (errno) {
            case ESRCH: {
                // process not found
                // throw exception?
                // for now pass through to default case
            }
            default: {
                return false;
            }
        }
    }
    return true;
}

// TODO: cleanup region code (especially related to mach_vm_*)
Process::Region Process::RegionForAddress(vm_address_t addr) {
    // attempt to retrieve mach task for process
    task_t task;
    if (!MachTask(&task)) {
        return Process::Region::null;
    }

    mach_vm_size_t vmsize = sizeof(vm_region_basic_info_data_64_t);
    vm_region_flavor_t flavor = VM_REGION_BASIC_INFO;
    vm_region_basic_info_data_64_t info;
    mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;
    memory_object_name_t object;
    mach_vm_address_t addr_info = addr;
    kern_return_t res = KERN_SUCCESS;
    if ((res = mach_vm_region(task, &addr_info, &vmsize, flavor,
                              (vm_region_info_t) &info, &info_count, &object))) {
        printf("%s\n", mach_error_string(res));
        return Process::Region::null;
    }

    return Process::Region(addr, vmsize, info.protection);
}

std::vector<Process::Region> Process::GetRegions(vm_prot_t prot) {
    std::vector<Process::Region> regions;

    // attempt to retrieve mach task
    task_t task;
    if (!MachTask(&task)) {
        return std::vector<Process::Region>();
    }

    kern_return_t status = KERN_SUCCESS;
    vm_address_t address = 0x0;
    uint32_t depth = 1;
    mach_vm_size_t size = 0;
    mach_vm_address_t addr_info = address; // remove me
    while (status == KERN_SUCCESS) {
        struct vm_region_submap_info_64 info;
        mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;

        status = mach_vm_region_recurse(task, &addr_info, &size, &depth,
                                        (vm_region_info_t) &info, &count);
        if (status == KERN_INVALID_ADDRESS) break;

        if (info.is_submap) {
            depth++;
            continue;
        }
        if (info.protection == prot) {
            regions.push_back(Process::Region(addr_info, size, info.protection));
        }
        addr_info += size;
    }

    return regions;

//    // attempt to retrieve mach task
//    task_t task;
//    if (!MachTask(&task)) {
//        return std::vector<Process::Region>();
//    }
//
//    enum Architecture arch = Architecture();
//
//    kern_return_t status = KERN_SUCCESS;
//    vm_address_t address = 0x0;
//    uint32_t depth = 1;
//    mach_vm_size_t size = 0;
//
//    while (status == KERN_SUCCESS) {
//        switch (arch) {
//            case Architecture::x86:
//            case Architecture::ARMv7: {
//                struct vm_region_submap_info info;
//                mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT;
//
//                status = mach_vm_region_recurse(task, (mach_vm_address_t *) &address, &size, &depth,
//                                                (vm_region_info_t) &info, &count);
//                if (status == KERN_INVALID_ADDRESS) break;
//
//                if (info.is_submap) {
//                    depth++;
//                    continue;
//                }
//                if (info.protection == prot) {
//                    regions.push_back(Process::Region(address, size, info.protection));
//                }
//                address += size;
//            }
//
//            case Architecture::x86_64:
//            case Architecture::AArch64: {
//                struct vm_region_submap_info_64 info;
//                mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;
//                status = vm_region_recurse_64(task, &address, (vm_size_t *) &size, &depth,
//                                              (vm_region_info_64_t) &info, &count);
//
//                if (status == KERN_INVALID_ADDRESS) break;
//
//                if (info.is_submap) {
//                    depth++;
//                    continue;
//                }
//                if (info.protection == prot) {
//                    regions.push_back(Process::Region(address, size, info.protection));
//                }
//                address += size;
//            }
//        }
//    }
    return regions;
}

bool Process::ModifyRegionProtection(vm_address_t addr, vm_prot_t new_prot) {
    Process::Region region = RegionForAddress(addr);
    return ModifyRegionProtection(region, new_prot);
}

bool Process::ModifyRegionProtection(const Process::Region &region, vm_prot_t new_prot) {
    task_t task;
    if (!MachTask(&task)) {
        return false;
    }
    return vm_protect(task, region.start(), region.size(), false, new_prot) == KERN_SUCCESS;
}

size_t Process::ReadMemory(uintptr_t address, uint8_t *out, size_t size) const {
    if (out) {
        task_t task;
        if (!MachTask(&task)) {
            return 0;
        }

        vm_size_t sz;
        kern_return_t res = vm_read_overwrite(task, address, size, (vm_address_t) out, &sz);
        if (res != KERN_SUCCESS) {
            // TODO: some exception?
        }
        return sz;
    } else {
        return 0;
    }
}

size_t Process::ReadMemory(uintptr_t address, std::vector<uint8_t> &out, size_t size) const {
    out.reserve(size);

    task_t task;
    if (!MachTask(&task)) {
        return false;
    }

    vm_size_t sz;
    kern_return_t res = vm_read_overwrite(task, address, size, (vm_address_t) &out[0], &sz);
    if (res != KERN_SUCCESS) {
        // TODO: some exception?
    }
    return sz;
}

size_t Process::WriteMemory(uintptr_t address, const uint8_t *bytes, size_t size, bool force) {
    task_t task;
    if (!MachTask(&task)) {
        return false;
    }

    if (force) {
        // TODO: use vm_region(_xx) to get original prot to restore later
        vm_prot_t rwx = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE | VM_PROT_COPY;
        if (vm_protect(task, address, size, false, rwx) != KERN_SUCCESS) {
            // could not change prot error
        }
    }

    if (vm_write(task, address, (vm_offset_t) bytes, size) != KERN_SUCCESS) {
        // could not write to address error
    }

    if (force) {
        // TODO: should restore original prot
        if (vm_protect(task, address, size, false, VM_PROT_READ | VM_PROT_EXECUTE) != KERN_SUCCESS) {
            // could not change prot error
        }
    }
    return size;
}

size_t Process::WriteMemory(uintptr_t address, const std::vector<uint8_t> &bytes, bool force) {
    task_t task;
    if (!MachTask(&task)) {
        return false;
    }
    size_t size = bytes.size();

    if (force) {
        // TODO: use vm_region(_xx) to get original prot to restore later
        vm_prot_t rwx = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE | VM_PROT_COPY;
        vm_protect(task, address, size, false, rwx);
    }

    vm_write(task, address, (vm_offset_t) bytes.data(), size);

    if (force) {
        vm_protect(task, address, size, false, VM_PROT_READ | VM_PROT_EXECUTE);
    }
    return size;
}

enum Architecture Process::Architecture() {
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, pid_};
    struct kinfo_proc kp;
    size_t bufsize = sizeof(kp);
    int err = sysctl(mib, sizeof(mib) / sizeof(int), &kp, &bufsize, NULL, 0);
    if (err != 0) return Architecture::UNKNOWN;

#if defined(__arm__) || defined(__arm64__)
    return (kp.kp_proc.p_flag & P_LP64) ? Architecture::AArch64 : Architecture::ARMv7;
#elif defined(__i386__) || defined(__x86_64__)
    return (kp.kp_proc.p_flag & P_LP64) ? Architecture::x86_64 : Architecture::x86;
#endif
    return Architecture::UNKNOWN;
}

pid_t Process::PIDForName(std::string name) {
    int proc_count = proc_listpids(1, 0, NULL, 0);
    pid_t *pids = new pid_t[proc_count];
    proc_listpids(1, 0, pids, sizeof(pids));

    for (int i = 0; i < proc_count; i++) {
        struct proc_bsdinfo proc;

        // this method sometimes requires root, may need to find a backup method
        proc_pidinfo(pids[i],
                     PROC_PIDTBSDINFO,
                     0,
                     &proc,
                     PROC_PIDTBSDINFO_SIZE);
        if (name == proc.pbi_name) {
            return pids[i];
        }
    }
    // not great returning a possible pid number but as its only used internally who cares
    return 0xFFFFFFFF;
}

std::string Process::NameForPID(pid_t pid) {
    struct proc_bsdinfo proc;

    // this method sometimes requires root, may need to find a backup method
    proc_pidinfo(pid,
                 PROC_PIDTBSDINFO,
                 0,
                 &proc,
                 PROC_PIDTBSDINFO_SIZE);
    // TODO: handle proc_pidinfo fail
    return proc.pbi_name;
}

std::string Process::PathForPID(pid_t pid) {
    char *buf = new char[MAXPATHLEN * 4];
    int ret = proc_pidpath(pid, buf, MAXPATHLEN * 4);
    if (ret != 0) {
        // error
    }

    std::string path = buf;
    delete[] buf;

    return path;
}

} // namespace masterlib
