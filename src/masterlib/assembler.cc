/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : assembler.cc                                                        *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <keystone/keystone.h>
#include "instr.h"
#include "assembler.h"

namespace masterlib {

using ArchModePair = ArchConfig::ArchModePair;

ArchModePair ArchConfig::KSConfig() {
    ArchModePair pair;
    switch (arch) {
        case x86:
        case x64: {
            pair.first = KS_ARCH_X86;
            break;
        }
        case ARM32: {
            pair.first = KS_ARCH_ARM;
            break;
        }
        case ARM64: {
            pair.first = KS_ARCH_ARM64;
            break;
        }
    }
    switch (mode) {
        case Mode32: {
            pair.second = (arch == x86) ? KS_MODE_32 : KS_MODE_ARM;
            break;
        }
        case Mode64: {
            pair.second = KS_MODE_64;
            break;
        }
        case ModeTHUMB: {
            pair.second = KS_MODE_THUMB;
            break;
        }
    }
    return pair;
}

// assembler code

Assembler::Assembler(std::string instr, uintptr_t addr, ArchConfig config) :
        instr_(instr), addr_(addr), config_(config), ks_(nullptr) {}

int Assembler::Setup() {
    ArchModePair pair = config_.KSConfig();
    return ks_open((ks_arch) pair.first, (ks_mode) pair.second, &ks_);
}

InstrRef Assembler::Assemble() {
    size_t count;
    unsigned char *encode;
    size_t size;

    if (ks_asm(ks_, instr_.data(), addr_, &encode, &size, &count) !=
        KS_ERR_OK) {
        return nullptr;
    } else {
        return std::make_unique<KeystoneInstr>(instr_, addr_, encode, size);
    }
}

} // namespace masterlib