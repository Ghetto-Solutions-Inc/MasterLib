//
//  ARMv7ThreadState.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include <sys/types.h>
#include "thread_state.h"

#if defined(__arm__) || defined(__arm64__)

class ARMv7ThreadState : public ThreadState {
 public:
  ARMv7ThreadState(mach_port_t thread) : ThreadState(thread) {}

  virtual std::string Description() override;
  virtual bool Load() override;
  virtual bool Save() override;
  virtual vm_address_t CurrentAddress() override;

 private:
  bool IsThumbMode();

  arm_thread_state_t thread_state_;

  // TODO: find out which of these states contains which float reg
  arm_vfp_state_t vfp_state_;
  arm_neon_state_t neon_state_;

  arm_exception_state_t exception_state_;
  arm_debug_state_t debug_state_;
};

#endif
