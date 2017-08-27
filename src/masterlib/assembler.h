/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : assembler.h                                                         *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef MASTERLIB_ASSEMBLER_H
#define MASTERLIB_ASSEMBLER_H

#include <string>
#include <vector>
#include <keystone/keystone.h>
#include <sstream>
#include "instr.h"

namespace masterlib {

inline std::vector<std::string> split(std::string s, std::string delimiter) {
    std::stringstream stream(s);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(stream, segment, '_'))
    {
        seglist.push_back(segment);
    }
    return seglist;
}

/**
 * Keystone-based implementation of the Instr class
 * due to the way keystone is constructed, this class may hod more than one
 * instruction but only the first one is used for the getter methods
 */
class KeystoneInstr : public Instr {
public:
    KeystoneInstr(std::string instr, uintptr_t addr, unsigned char *bytes, size_t size)
            : instr_(instr), addr_(addr), bytes_(bytes, bytes+size) {}

    uint64_t address() const override { return addr_; }
    uint16_t size() const override { return bytes_.size(); }

    const char *mnemonic() const {
        return split(instr_, " ")[0].c_str();
    }

    const char *operands() const {
        auto data = split(instr_, " ");
        data.erase(data.begin());

        std::string operands;
        for (std::string &s: data) operands += s;
        return operands.data();
    }

    std::vector<byte> bytes() const override {
        return bytes_;
    }

    std::vector<uint8_t> read_regs() const override {
        return std::vector<uint8_t>(); // keystone doesn't support this
    }

    std::vector<uint8_t> write_regs() const override {
        return std::vector<uint8_t>(); // keystone doesn't support this
    }

private:
    std::string instr_;
    uintptr_t addr_;
    std::vector<byte> bytes_;
};

class Assembler {
public:
    Assembler(std::string instr, uintptr_t addr, ArchConfig config = HOST_CONFIG);

    int Setup();

    InstrRef Assemble();

private:
    std::string instr_;
    uintptr_t addr_;
    ArchConfig config_;
    ks_engine *ks_;
};

} // namespace masterlib

#endif //MASTERLIB_ASSEMBLER_H
