//
//  NOPBreakpoint.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "Breakpoint.h"

class NOPBreakpoint : public Breakpoint {
 public:
  virtual bool Apply() override {
    throw std::runtime_error("NOP implementation called");
  };

  virtual bool Reset() override {
    throw std::runtime_error("NOP implementation called");
  };
};
