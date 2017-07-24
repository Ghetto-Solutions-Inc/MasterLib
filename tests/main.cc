#include <iostream>
#include <process.h>
#include <mach-o/dyld.h>
using masterlib::Process;

inline void print_region(const Process::Region &region) {
    std::cout << std::hex << region.start() << " "
              << std::hex << region.size() << " "
              << std::hex << region.prot() << std::endl;
}

int main() {
    Process *self = Process::self;

    std::cout << self->pid() << " " << self->name() << " " << self->path() << std::endl;

    auto region = self->RegionForAddress((vm_address_t)_dyld_get_image_header(0));

    print_region(region);
    std::cout << std::hex << _dyld_get_image_header(0) << std::endl;

    for (const auto& region: self->GetRegions()) {
        print_region(region);
    }

}