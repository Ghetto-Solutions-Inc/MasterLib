//
//  thread_state_x86.h
//  MasterLib
//
//  Created by satori
//  Copyright © 2017 satori. All rights reserved.
//

#pragma once


#include "thread_state.h"

class x86ThreadState : public ThreadState {
 public:
  x86ThreadState(mach_port_t thread) : ThreadState(thread) {}

  virtual std::string Description() override;
  virtual bool Load() override;
  virtual bool Save() override;
  virtual vm_address_t CurrentAddress() override;

  x86_thread_state32_t &thread_state() { return thread_state_; }
  x86_float_state32_t &float_state() { return float_state_; }
  x86_debug_state32_t &debug_state() { return debug_state_; }
  x86_exception_state32_t &exception_state() { return exception_state_; }

 private:
  x86_thread_state32_t thread_state_;
  x86_float_state32_t float_state_;
  x86_debug_state32_t debug_state_;
  x86_exception_state32_t exception_state_;
};
