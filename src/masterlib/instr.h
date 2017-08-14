/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : instr.h                                                             *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef MASTERLIB_INSTR_H
#define MASTERLIB_INSTR_H

#include <vector>

namespace masterlib {

using byte = uint8_t;

/**
 * Virtual interface for an object representing an instruction
 */
class Instr {
public:
    virtual uint64_t address() const = 0;
    virtual uint16_t size() const = 0;
    virtual std::vector<byte> bytes() const = 0;
    virtual const char *mnemonic() const = 0;
    virtual const char *operands() const = 0;

    // these may be capstone specific
    virtual std::vector<byte> read_regs() const = 0;
    virtual std::vector<byte> write_regs() const = 0;
};

} // namespace masterlib


#endif //MASTERLIB_INSTR_H
