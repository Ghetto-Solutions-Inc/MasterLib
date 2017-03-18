//
//  x86_64Breakpoint.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "breakpoint.h"

#if defined(__i386__) || defined(__x86_64__)  // we're targeting an x86 system

class x86_64HardwareBreakpoint : public Breakpoint {
 public:
  x86_64HardwareBreakpoint(Process *proc, vm_address_t addr)
  : Breakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;

 private:
  int in_use_;
};

class x86_64SoftwareBreakpoint : public Breakpoint {
 public:
  x86_64SoftwareBreakpoint(Process *proc, vm_address_t addr)
  : Breakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;

 private:
  std::vector<uint8_t> original_opcode_;
};

#else  // we're targeting something else (likely ARM)

#include "NOPBreakpoint.h"

using x86_64HardwareBreakpoint = NOPBreakpoint;
using x86_64SoftBreakpoint = NOPBreakpoint;

#endif /* _x86_64_ */
