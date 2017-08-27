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

using ArchModePair = ArchConfig::ArchModePair;

ArchModePair ArchConfig::CSConfig() {
    ArchModePair pair;
    switch (arch) {
        case x86:
        case x64: {
            pair.first = CS_ARCH_X86;
            break;
        }
        case ARM32: {
            pair.first = CS_ARCH_ARM;
            break;
        }
        case ARM64: {
            pair.first = CS_ARCH_ARM64;
            break;
        }
    }
    switch (mode) {
        case Mode32: {
            pair.second = (arch == x86) ? CS_MODE_32 : CS_MODE_ARM;
            break;
        }
        case Mode64: {
            pair.second = CS_MODE_64;
            break;
        }
        case ModeTHUMB: {
            pair.second = CS_MODE_THUMB;
            break;
        }
    }
    return pair;
}

// dissassembler code

Disassembler::Disassembler(uintptr_t addr, const uint8_t *code, size_t size, ArchConfig config)
        : setup_(false), address_(addr), code_(code), size_(size), config_(config) {}

Disassembler::~Disassembler() {
    cs_close(&handle_);
}

int Disassembler::Setup() {
    setup_ = true;
    ArchModePair pair = config_.CSConfig();
    int res = cs_open((cs_arch)pair.first, (cs_mode)pair.second, &handle_);
    cs_option(handle_, CS_OPT_DETAIL, CS_OPT_ON);
    return res;
}

InstrRef Disassembler::Disassemble(size_t off) {
    if (setup_) {
        cs_insn *insn;
        cs_disasm(handle_, code_ + off, size_, address_ + off, 0, &insn);

        return std::make_unique<CapstoneInstr>(insn);
    } else {
        Setup();
        return Disassemble(off);
    }
}

InstrRef Disassembler::DisassembleNext(InstrRef current) {
    size_t offset = current->address() - address_;
    offset += current->size();
    return Disassemble(offset);
}

void Disassembler::DisassembleIter(const IterCallback &cb) {
    if (setup_) {
        size_t size = size_;
        const uint8_t *code = code_;
        uint64_t address = address_;
        cs_insn *insn = cs_malloc(handle_);

        while (cs_disasm_iter(handle_, &code, &size, &address, insn)) {
            if (cb) {
                InstrRef instr = std::make_unique<CapstoneInstr>(insn);
                if (!cb(instr)) {
                    // callback returning false indicates the user wants to end iteration
                    break;
                }
            }
        }

        cs_free(insn, 1);
    } else {
        Setup();
        DisassembleIter(cb);
    }
}

} // namespace masterlib