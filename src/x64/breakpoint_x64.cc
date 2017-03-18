//
//  x86_64Breakpoint.cc
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#include <thread>
#include "host.h"
#include "process.h"
#include "breakpoint_x64.h"
#include "thread_state_x64.h"

// structure representing the debug register dr7
struct dr7_ctx {
  uint8_t flags_dr0 : 2;
  uint8_t flags_dr1 : 2;
  uint8_t flags_dr2 : 2;
  uint8_t flags_dr3 : 2;

  char unused;

  uint8_t trigger_dr0 : 2;
  uint8_t size_dr0 : 2;
  uint8_t trigger_dr1 : 2;
  uint8_t size_dr1 : 2;
  uint8_t trigger_dr2 : 2;
  uint8_t size_dr2 : 2;
  uint8_t trigger_dr3 : 2;
  uint8_t size_dr3 : 2;

  operator int() { return *(int *)this; }
  void operator=(int val) { memcpy(this, &val, sizeof(int)); }
  dr7_ctx(int val) { memcpy(this, &val, sizeof(int)); }
};

// TODO: MAJOR cleanup of the below code
bool x86_64HardwareBreakpoint::Apply() {
  mach_port_t mach_thread = mach_thread_self();
  std::vector<ThreadState *> threads = proc_->Threads(mach_thread);

  for (int i = 0; i < threads.size(); i++) {
    x64ThreadState *state = dynamic_cast<x64ThreadState *>(threads[i]);
    state->Load();

    x86_debug_state64_t &debug = state->debug_state();
    dr7_ctx dr7 = debug.__dr7;
    switch (in_use_) {
      case 0: {
        dr7.flags_dr0 |= 0x1;    // local
        dr7.size_dr0 |= 0x0;     // 1 byte breakpoint
        dr7.trigger_dr0 |= 0x0;  // trigger on code exec

        debug.__dr0 = address_;
        break;
      }

      case 1: {
        dr7.flags_dr1 |= 0x1;    // local
        dr7.size_dr1 |= 0x0;     // 1 byte breakpoint
        dr7.trigger_dr1 |= 0x0;  // trigger on code exec

        debug.__dr1 = address_;
        break;
      }

      case 2: {
        dr7.flags_dr2 |= 0x1;    // local
        dr7.size_dr2 |= 0x0;     // 1 byte breakpoint
        dr7.trigger_dr2 |= 0x0;  // trigger on code exec

        debug.__dr2 = address_;
        break;
      }

      case 3: {
        dr7.flags_dr3 |= 0x1;    // local
        dr7.size_dr3 |= 0x0;     // 1 byte breakpoint
        dr7.trigger_dr3 |= 0x0;  // trigger on code exec

        debug.__dr3 = address_;
        break;
      }

      default: {
        // all debug registers in use
        return false;
      }
    }
    debug.__dr7 = dr7;
    in_use_++;
    active_ = true;
    state->Save();
  }
  return true;
}

bool x86_64HardwareBreakpoint::Reset() {
  Host *host = Host::CurrentHost();

  pthread_t handle = pthread_self();
  mach_port_t mach_thread = pthread_mach_thread_np(handle);
  std::vector<ThreadState *> threads = proc_->Threads(mach_thread);

  for (int i = 0; i < threads.size(); i++) {
    x64ThreadState *state = dynamic_cast<x64ThreadState *>(threads[i]);
    state->Load();
    x86_debug_state64_t &debug_state_ = state->debug_state();

    int index = -1;
    if (debug_state_.__dr0 == address_) index = 0;
    if (debug_state_.__dr1 == address_) index = 1;
    if (debug_state_.__dr2 == address_) index = 2;
    if (debug_state_.__dr3 == address_) index = 3;

    if (index == -1) {
      continue;
    }

    dr7_ctx dr7 = debug_state_.__dr7;

    switch (index) {
      case 0: {
        dr7.flags_dr0 = 0;
        break;
      }

      case 1: {
        dr7.flags_dr1 = 0;
        break;
      }

      case 2: {
        dr7.flags_dr2 = 0;
        break;
      }

      case 3: {
        dr7.flags_dr3 = 0;
        break;
      }
    }

    debug_state_.__dr7 = dr7;
    active_ = false;
    state->Save();
  }
  return true;
}

bool x86_64SoftwareBreakpoint::Apply() {
  active_ = true;
  static uint8_t opcode[] = {0xCC};

  uint8_t original[1];

  if (proc_->ReadMemory(address_, (char *)original, 1)) return false;

  original_opcode_.push_back(*original);

  return proc_->WriteMemory(address_, (char *)opcode, 1, true);
}

bool x86_64SoftwareBreakpoint::Reset() {
  active_ = false;
  return proc_->WriteMemory(address_, (char *)original_opcode_.data(),
                            original_opcode_.size(), true);
}
