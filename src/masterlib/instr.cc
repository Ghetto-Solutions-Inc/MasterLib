/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : instr.cc                                                            *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "instr.h"

 namespace masterlib {

 const ArchConfig ArchConfig::x86Config{x86, Mode32};
 const ArchConfig ArchConfig::x64Config{x86, Mode64};
 const ArchConfig ArchConfig::ARMConfig{ARM32, Mode32};
 const ArchConfig ArchConfig::THUMBConfig{ARM32, ModeTHUMB};
 const ArchConfig ArchConfig::ARM64Config{ARM64, Mode64};

 } // namespace masterlib
