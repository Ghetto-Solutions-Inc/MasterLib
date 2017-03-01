//
//  disassembler.cpp
//  masterlib
//
//  Created by callum taylor on 01/03/2017.
//
//

#include "disassembler.h"


Disassembler::Disassembler(uintptr_t addr, const uint8_t *code, size_t size, Arch arch, Mode mode)
: address_(addr), code_(code), size_(size), arch_(arch), mode_(mode) {}

Disassembler::~Disassembler() {
    cs_close(&handle_);
}

int Disassembler::Setup() {
    return cs_open((cs_arch)arch_, (cs_mode)mode_, &handle_);
}

Instr Disassembler::Disassemble() {
    cs_insn *insn;
    cs_disasm(handle_, code_, size_, address_, 0, &insn);
    
    return Instr(insn);
}
