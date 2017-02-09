//
//  x86_64ThreadState.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "thread_state.h"

#if defined(__i386__) || defined(__x86_64__)  // we're targeting an x86 system

extern const char *thread_registers[];
extern const char *debug_registers[];

class x86_64ThreadState : public ThreadState {
 public:
  x86_64ThreadState(mach_port_t thread) : ThreadState(thread) {}

  virtual std::string Description() override;
  virtual bool Load() override;
  virtual bool Save() override;
  virtual vm_address_t CurrentAddress() override;

  x86_thread_state64_t &thread_state() { return thread_state_; }
  x86_float_state64_t &float_state() { return float_state_; }
  x86_debug_state64_t &debug_state() { return debug_state_; }
  x86_exception_state64_t &exception_state() { return exception_state_; }

 private:
  x86_thread_state64_t thread_state_;
  x86_float_state64_t float_state_;
  x86_debug_state64_t debug_state_;
  x86_exception_state64_t exception_state_;
};

#else  // we're targeting something else (likely ARM)

#endif /* _x86_64_ */
