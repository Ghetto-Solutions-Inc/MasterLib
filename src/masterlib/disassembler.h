/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : disassembler.h                                                      *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef MASTERLIB_DISASSEMBLER_H
#define MASTERLIB_DISASSEMBLER_H

#include <functional>
#include <memory>
#include <capstone.h>
#include "instr.h"

namespace masterlib {

#if defined(__x86_64__)
#define HOST_ARCH Arch::x64
#define HOST_MODE Mode::Mode64
#elif defined(__i386__)
#define HOST_ARCH Arch::x86
#define HOST_MODE Mode::Mode32
#elif defined(__arm64__)
#define HOST_ARCH Arch::ARM64
#define HOST_MODE Mode::Mode64
#elif defined(__arm__)
#define HOST_ARCH Arch::ARM
#define HOST_MODE Mode::ModeTHUMB
#else
#error building for an invalid arch
#endif

/**
 * capstone based implementation of Instr class
 * worth moving to its own file?
 */
class CapstoneInstr : public Instr {
    friend class Disassembler;
public:
    CapstoneInstr(cs_insn *instr) : instr_(instr) {}

    uint64_t address() const override { return instr_->address; }
    uint16_t size() const override { return instr_->size; }
    const char *mnemonic() const override { return instr_->mnemonic; }
    const char *operands() const override { return instr_->op_str; }

    std::vector<byte> bytes() const override {
        return std::vector<byte>(instr_->bytes, instr_->bytes + size());
    }

    std::vector<uint8_t> read_regs() const override {
        auto vec = std::vector<uint8_t>();
        auto detail = instr_->detail;
        if (detail) {
            auto vec = std::vector<uint8_t>();
            vec.assign(detail->regs_read, detail->regs_read + detail->regs_read_count);
        }
        return vec;
    }

    std::vector<uint8_t> write_regs() const override {
        auto detail = instr_->detail;
        auto vec = std::vector<uint8_t>();
        if (detail) {
            vec.assign(detail->regs_write, detail->regs_write + detail->regs_write_count);
        }
        return vec;
    }

    cs_insn *instr_;
};


using IterCallback = std::function<Instr &>;

enum class Arch {
    x86 = CS_ARCH_X86,
    x64 = CS_ARCH_X86,
    ARM = CS_ARCH_ARM,
    ARM64 = CS_ARCH_ARM64
};

enum class Mode {
    Mode32 = CS_MODE_32,
    Mode64 = CS_MODE_64,
    ModeARM = CS_MODE_ARM,
    ModeTHUMB = CS_MODE_THUMB
};

class Disassembler {
public:
    Disassembler(uintptr_t addr, const uint8_t *code, size_t size, Arch arch = HOST_ARCH, Mode mode = HOST_MODE);
    ~Disassembler();

    int Setup();

    std::unique_ptr<Instr> Disassemble();
    void DisassembleIter(IterCallback &cb);

private:
    uintptr_t address_;
    const uint8_t *code_;
    size_t size_;
    Arch arch_;
    Mode mode_;
    csh handle_;
};

} // namespace masterlib

#endif //MASTERLIB_DISASSEMBLER_H
