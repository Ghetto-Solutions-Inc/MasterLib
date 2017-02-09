//
//  AArch64Breakpoint.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "Breakpoint.h"

#if defined(__arm__) || defined(__arm64__)

class AArch64HardwareBreakpoint : public Breakpoint {
 public:
  AArch64HardwareBreakpoint(Process *proc, vm_address_t addr)
  : Breakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;
};

class AArch64SoftwareBreakpoint : public Breakpoint {
 public:
  AArch64SoftwareBreakpoint(Process *proc, vm_address_t addr)
  : Breakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;

 private:
  std::vector<uint8_t> original_opcode_;
};

#else

#include "NOPBreakpoint.h"
using AArch64HardwareBreakpoint = NOPBreakpoint;
using AArch64SoftwareBreakpoint = NOPBreakpoint;

#endif
