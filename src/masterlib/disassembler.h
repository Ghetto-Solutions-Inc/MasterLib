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

#include <capstone.h>
#include <memory>
#include <functional>
#include "instr.h"

namespace masterlib {

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

using IterCallback = std::function<bool(InstrRef &)>;

/**
 * Provides a basic wrapper around capstone.
 * Could be improved a lot.
 */
class Disassembler {
public:
    Disassembler(uintptr_t addr, const uint8_t *code, size_t size, ArchConfig config = HOST_CONFIG);
    ~Disassembler();

    /**
     * Sets up the disassembler and underlying capstone object for use.
     * Currently this gets called automatically if another function
     * is called but that may change in the future
     * @return capstone return code
     */
    int Setup();

    /**
     * Disassemble the currently stored code
     * @param off offset used to provide accurate PC-rel disassembly
     * @return smart pointer to Instr object (or nullptr if disassembly failed)
     */
    InstrRef Disassemble(size_t off = 0);

    /**
     * Disassemble next instruction from passed instruction
     * @param current current instruction
     * @return smart pointer to Instr object (or nullptr if disassembly failed)
     */
    InstrRef DisassembleNext(InstrRef current);

    /**
     * Disassemble instructions passing the Instr objects to a callback function
     * Disassembly continues until callback function returns false
     * @param cb callback function
     */
    void DisassembleIter(const IterCallback &cb);

    // getters

    bool setup() const { return setup_; }

    uintptr_t address() const { return address_; }
    void set_address(uintptr_t address) { address_ = address; }

    const uint8_t *code() const { return code_; }
    void set_code(const uint8_t *code) { code_ = code; }

    const ArchConfig &config() const { return config_; }

    csh handle() { return handle_; }

private:
    bool setup_;
    uintptr_t address_;
    const uint8_t *code_;
    size_t size_;
    ArchConfig config_;
    csh handle_;
};

} // namespace masterlib

#endif //MASTERLIB_DISASSEMBLER_H
