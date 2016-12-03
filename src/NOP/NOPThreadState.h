//
//  NOPThreadState.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "ThreadState.h"

class NOPThreadState : public ThreadState {
 public:
  NOPThreadState(mach_port_t thread) : ThreadState(thread) {}

  virtual std::string Description() {
    throw std::runtime_error("NOP implementation called");
  };

  virtual bool Load() override {
    throw std::runtime_error("NOP implementation called");
  };

  virtual bool Save() override {
    throw std::runtime_error("NOP implementation called");
  };

  virtual vm_address_t CurrentAddress() override {
    throw std::runtime_error("NOP implementation called");
  };
};
