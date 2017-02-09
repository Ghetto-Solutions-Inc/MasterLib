//
//  ARMv7Breakpoint.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "Breakpoint.h"

#if defined(__arm__) || defined(__arm64__)

class ARMv7HardwareBreakpoint : public Breakpoint {
 public:
  ARMv7HardwareBreakpoint(Process *proc, vm_address_t addr)
  : Breakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;
};

class ARMv7SoftwareBreakpoint : public Breakpoint {
 public:
  ARMv7SoftwareBreakpoint(Process *proc, vm_address_t addr)
  : Breakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;

 protected:
  std::vector<uint8_t> original_opcode_;
};

class THUMBv7SoftwareBreakpoint : public ARMv7SoftwareBreakpoint {
 public:
  THUMBv7SoftwareBreakpoint(Process *proc, vm_address_t addr)
  : ARMv7SoftwareBreakpoint(proc, addr) {}

  virtual bool Apply() override;
  virtual bool Reset() override;
};

#else

#include "NOPBreakpoint.h"

using ARMv7HardwareBreakpoint = NOPBreakpoint;
using ARMv7SoftwareBreakpoint = NOPBreakpoint;
using THUMBv7SoftwareBreakpoint = NOPBreakpoint;

#endif
