//
//  AArch64ThreadState.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include <sys/types.h>
#include "thread_State.h"

#if defined(__arm__) || defined(__arm64__)

class AArch64ThreadState : public ThreadState {
 public:
  AArch64ThreadState(mach_port_t port) : ThreadState(port) {}

  virtual std::string Description() override;
  virtual bool Load() override;
  virtual bool Save() override;
  virtual vm_address_t CurrentAddress() override;

 private:
  arm_thread_state64_t thread_state_;
  arm_neon_state64_t neon_state_;
  arm_exception_state64_t exception_state_;
  arm_debug_state64_t debug_state_;
};

#else

#include "NOPThreadState.h"

using AArch64ThreadState = NOPThreadState;

#endif
