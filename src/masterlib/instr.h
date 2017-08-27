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

struct ArchConfig {
    using ArchModePair = std::pair<int, int>;

    enum Arch: uint8_t {
        x86,
        x64,
        ARM32,
        ARM64
    };

    enum Mode: uint8_t {
        Mode32,
        Mode64,
        ModeTHUMB
    };

    static const ArchConfig x86Config;
    static const ArchConfig x64Config;
    static const ArchConfig ARMConfig;
    static const ArchConfig THUMBConfig;
    static const ArchConfig ARM64Config;

    Arch arch;
    Mode mode;

    ArchModePair CSConfig();
    ArchModePair KSConfig();

//    int arch_as_cs();
//    int arch_as_ks();
//    int mode_as_cs();
//    int mode_as_ks();
};

const inline ArchConfig ArchConfig::x86Config{x86, Mode32};
const inline ArchConfig ArchConfig::x64Config{x86, Mode64};
const inline ArchConfig ArchConfig::ARMConfig{ARM32, Mode32};
const inline ArchConfig ArchConfig::THUMBConfig{ARM32, ModeTHUMB};
const inline ArchConfig ArchConfig::ARM64Config{ARM64, Mode64};

// TODO: make these constexpr in ArchConfig
#if defined(__x86_64__)
#define HOST_CONFIG ArchConfig::x64Config
#elif defined(__i386__)
#define HOST_CONFIG ArchConfig::x86Config
#elif defined(__arm64__)
#define HOST_CONFIG ArchConfig::ARM64Config
#elif defined(__arm__)
#define HOST_CONFIG ArchConfig::ARMConfig
#else
#error building for an invalid arch
#endif

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

using InstrRef = std::unique_ptr<Instr>;

} // namespace masterlib


#endif //MASTERLIB_INSTR_H
