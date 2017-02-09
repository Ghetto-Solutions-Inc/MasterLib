//
//  ARMv7Breakpoint.cc
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#include "breakpoint_armv7.h"
#include "process.h"
#include "thread_state.h"

bool ARMv7HardwareBreakpoint::Apply() {
  // if (HardwareSlotsLeft()) {
  //   ARMv7ThreadState &state = proc->Threads()[0];
  // }
  return false;
}

bool ARMv7HardwareBreakpoint::Reset() {
  return false;
}

bool ARMv7SoftwareBreakpoint::Apply() {
  // may need to reverse endian
  static uint8_t opcode[] = {0x70, 0x00, 0x20, 0xE1};

  uint8_t original[4];

  if (proc_->ReadMemory(address_, (char *)original, 4)) return false;

  original_opcode_.assign(original, original + 4);

  return proc_->WriteMemory(address_, (char *)opcode, sizeof(opcode), true);
}

bool ARMv7SoftwareBreakpoint::Reset() {
  return proc_->WriteMemory(address_, (char *)original_opcode_.data(),
                            original_opcode_.size(), true);
}

bool THUMBv7SoftwareBreakpoint::Apply() {
  // may need to reverse endian
  static uint8_t opcode[] = {0x00, 0xBE};

  uint8_t original[2];

  if (proc_->ReadMemory(address_, (char *)original, 2)) return false;

  original_opcode_.assign(original, original + 2);

  return proc_->WriteMemory(address_, (char *)opcode, sizeof(opcode), true);
}

bool THUMBv7SoftwareBreakpoint::Reset() {
  return ARMv7SoftwareBreakpoint::Reset();
}
