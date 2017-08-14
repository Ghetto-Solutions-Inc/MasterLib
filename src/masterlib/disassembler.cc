/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : disassembler.cc                                                     *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "disassembler.h"

namespace masterlib {

Disassembler::Disassembler(uintptr_t addr, const uint8_t *code, size_t size, Arch arch, Mode mode)
        : address_(addr), code_(code), size_(size), arch_(arch), mode_(mode) {}

Disassembler::~Disassembler() {
    cs_close(&handle_);
}

int Disassembler::Setup() {
    return cs_open((cs_arch)arch_, (cs_mode)mode_, &handle_);
}

std::unique_ptr<Instr> Disassembler::Disassemble() {
    cs_insn *insn;
    cs_disasm(handle_, code_, size_, address_, 0, &insn);

    return std::make_unique<CapstoneInstr>(insn);
}

void Disassembler::DisassembleIter(IterCallback &cb) {

}

} // namespace masterlib