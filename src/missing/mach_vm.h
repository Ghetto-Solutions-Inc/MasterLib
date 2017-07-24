#ifndef MASTERLIB_MACH_VM_H_H
#define MASTERLIB_MACH_VM_H_H

#include <TargetConditionals.h>

// On iOS 5 and higher, mach/mach_vm.h is not supported. Use the corresponding
// vm_map functions instead.
#if TARGET_OS_IPHONE
#include <mach/vm_map.h>
#define mach_vm_address_t vm_address_t
#define mach_vm_deallocate vm_deallocate
#define mach_vm_read vm_read
#if defined(__LP64__)
#define mach_vm_region_recurse vm_region_recurse_64
#define mach_vm_region vm_region_64
#else
#define mach_vm_region_recurse vm_region_recurse
#define mach_vm_region vm_region
#endif
#define mach_vm_size_t vm_size_t
#else
#include <mach/mach_vm.h>
#endif  // TARGET_OS_IPHONE

#endif //MASTERLIB_MACH_VM_H_H
