#include <iostream>
#include <iomanip>
#include <masterlib/process.h>
#include <masterlib/disassembler.h>
#include <masterlib/assembler.h>
#include <mach-o/dyld.h>


using namespace masterlib;

inline void print_region(const Process::Region &region) {
    std::cout << std::hex << region.start() << " "
              << std::hex << region.size() << " "
              << std::hex << region.prot() << std::endl;
}

inline void print_instr(const InstrRef &instr) {
    std::cout << instr->mnemonic()
              << " "
              << instr->operands()
              << " ";
    for (auto &b: instr->bytes()) {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)b;
    }
    std::cout << std::endl;
}

int main() {
    auto self = Process::self;

    std::cout << self->pid() << " " << self->name() << " " << self->path() << std::endl;

    auto region = self->RegionForAddress((vm_address_t)_dyld_get_image_header(0));

    print_region(region);
    std::cout << std::hex << _dyld_get_image_header(0) << std::endl << "--------" << std::endl;

    for (const auto& region: self->GetRegions(VM_PROT_READ | VM_PROT_EXECUTE)) {
        print_region(region);
    }

    const uint8_t code[] = {
            0x00, 0x00, 0xA0, 0xE1,
            0x00, 0xFF, 0xA0, 0xE3,
            0x1E, 0xFF, 0x2F, 0xE1
    };
    Disassembler ds(0x0, code, sizeof(code), ArchConfig::ARMConfig);
    ds.Setup();

    InstrRef instr = ds.Disassemble();

    //print_instr(instr);

    ds.DisassembleIter([](InstrRef &instr) {
        //print_instr(instr);
        return true;
    });

    Assembler as("MOV R0, R1", 0x0, ArchConfig::ARMConfig);
    as.Setup();

    InstrRef assembled = as.Assemble();
    print_instr(assembled);
}