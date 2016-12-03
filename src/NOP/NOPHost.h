//
//  NOPHost.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include <stdexcept>
#include "Host.h"

class NOPHost : public Host {
 public:
  int HardwareBreakpointCount() override {
    throw std::runtime_error("NOP implementation called");
  }

  int HardwareWatchpointCount() override {
    throw std::runtime_error("NOP implementation called");
  }

  enum Platform Platform() const { return Platform::UNKNOWN; }
};
