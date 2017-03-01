//
//  disassembler.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include <sys/types.h>
#include <third_party/capstone/include/capstone.h>
#include <functional>
#include "instr.h"

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
    
    Instr Disassemble();
    void DIsassembleIter(IterCallback &cb);
protected:
    uintptr_t address_;
    const uint8_t *code_;
    size_t size_;
    Arch arch_;
    Mode mode_;
    csh handle_;
};
