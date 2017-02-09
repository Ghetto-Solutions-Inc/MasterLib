//
//  AArch64Host.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "Host.h"

#if defined(__arm__) || defined(__arm64__)

class AArch64Host : public Host {
 public:
  virtual int HardwareBreakpointCount() override;
  virtual int HardwareWatchpointCount() override;  // unusued in liberation
  virtual enum Platform Platform() const override { return Platform::AArch64; }
};

#else

#include "NOPHost.h"
using AArch64Host = NOPHost;

#endif
