/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : process.h                                                           *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */


#ifndef MASTERLIB_PROCESS_H
#define MASTERLIB_PROCESS_H

#include <mach/mach.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <vector>

namespace masterlib {

enum class Architecture : unsigned {
    x86,
    x86_64,
    ARMv7,
    AArch64,
    UNKNOWN = 0xFFFFFFFF
};

/**
 * Provides methods to access and manipulate running processes on the system
 */
class Process {
public:
    /**
     * Represents a region of memory in a process
     */
    class Region {
        friend class Process;
        static Region null;
    public:

        /**
         * Getters
         */

        vm_address_t start() const { return start_; }
        vm_address_t size() const { return size_; }
        vm_prot_t prot() const { return prot_; }

        /**
         * Check if region has memory protection
         * @param prot protection to test
         * @return whether region has the protection
         */
        bool HasProtection(vm_prot_t prot) const {
            return prot_ & prot;
        }

        /**
         * Check if a region definition is valid
         * @return whether region struct is valid;
         */
        bool IsValid() const {
            return size_ > 0;
        }

    protected:
        vm_address_t start_;
        vm_address_t size_;
        vm_prot_t prot_;

        Region(vm_address_t start, vm_address_t size, vm_prot_t prot)
                : start_(start), size_(size), prot_(prot) {}
    };

    /**
     * A Process instance representing this process
     */
    static Process *self;

    /**
     * Return a pointer to process instance with passed pid
     * @param pid pid of process
     * @return pointer to process object
     */
    static std::unique_ptr<Process> FindProcess(pid_t pid);

    /**
     * Return a pointer to process instance with passed name
     * If multiple processes exist with the same name the first one is returned
     * @param name name of process
     * @return pointer to process object
     */
    static std::unique_ptr<Process> FindProcess(std::string name);

    /**
     * getters
     */

    pid_t pid() const { return pid_; }

    std::string name() const { return name_; }

    std::string path() const { return path_; }

    /**
     * Get the mach task for the process
     * If task can be retrieved, it is stored in out param
     * @param out pointer to where to store task
     * @return true if task was retrieved, false otherwise
     */
    bool MachTask(task_t *out) const;

    /**
     * Checks whether the process is alive
     * This method should be used sparingly as it's possible for the process to die
     * after this call but before the result is used
     * @return whether the process is alive
     */
    bool IsAlive() const;

    /**
     * Kill the process
     * @return true if process was killed, false otherwise
     */
    bool Kill();

    /**
     * Return a Region structure for the region containing addr
     * @param addr address to get region for
     * @return region structure
     */
    Process::Region RegionForAddress(vm_address_t addr);

    /**
     * Get all regions in process
     * This is not thread safe
     * @return list of all current regions in process
     */
    std::vector<Process::Region> GetRegions(vm_prot_t prot = VM_PROT_READ | VM_PROT_WRITE);

    /**
     * Modify target process region memory protections
     * @param addr address of region to modify
     * @param new_prot new protection to apply
     * @return if modification was successful
     */
    bool ModifyRegionProtection(vm_address_t addr, vm_prot_t new_prot);

    /**
     * Modify target process region memory protections
     * @param region region to modify
     * @param new_prot new protection to apply
     * @return if modification was successful
     */
    bool ModifyRegionProtection(const Process::Region &region, vm_prot_t new_prot);

    /**
     * Read a block of memory from the process
     * @param address address to start reading from
     * @param out pre-allocated byte array to fill
     * @param size how many bytes to read
     * @return the number of bytes read
     */
    size_t ReadMemory(uintptr_t address, uint8_t *out, size_t size) const;

    /**
     * Read a block of memory from the process
     * @param address address to start reading from
     * @param out vector to fill with read data
     * @param size how many bytes to read
     * @return the number of bytes read
     */
    size_t ReadMemory(uintptr_t address, std::vector<uint8_t> &out, size_t size) const;

    /**
     * Write data to the memory of the process
     * @param address address to write to
     * @param bytes array of bytes to write
     * @param size number of bytes to write
     * @param force whether to force the write (write over read-only sections)
     * @return number of bytes written
     */
    size_t WriteMemory(uintptr_t address, const uint8_t *bytes, size_t size, bool force = false);

    /**
     * Write data to the memory of the process
     * @param address address to write to
     * @param bytes vector of bytes to write
     * @param force whether to force the write (write over read-only sections)
     * @return number of bytes written
     */
    size_t WriteMemory(uintptr_t address, const std::vector<uint8_t> &bytes, bool force = false);

    /**
     * Target process' running architecture
     * @return architecture enum
     */
    enum Architecture Architecture(); // TODO: make a member variable?

private:
    /**
     * Constructs Process::self before main()
     * @param argc
     * @param argv
     * @param envp
     * @param apple
     */
    static void ConstructSelfProcess(int argc,
                                     const char **argv,
                                     const char **envp,
                                     const char **apple,
                                     struct ProgramVars *);

    /**
     * Gets process-id for process with supplied name
     * If multiple processes with the same name exists the first pid found is returned
     * @param name supplied process name
     * @return pid for process
     */
    static pid_t PIDForName(std::string name);

    /**
     * Gets process name for process with supplied pid
     * @param pid pid of the process
     * @return name of the process
     */
    static std::string NameForPID(pid_t pid);

    /**
     * Gets process path for process with supplied pid
     * @param pid pid of the process
     * @return path of the process
     */
    static std::string PathForPID(pid_t pid);

    /**
     * constructs a new Process object
     * @param pid pid of process
     * @param name name of process
     * @param path path of process
     */
    Process(pid_t pid, std::string name, std::string path)
            : pid_(pid), name_(name), path_(path) {}

    pid_t pid_;
    std::string name_;
    std::string path_;
};

} // namespace masterlib

#endif //MASTERLIB_PROCESS_H
