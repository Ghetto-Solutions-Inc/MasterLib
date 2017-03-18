//
//  x86_64Host.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "host.h"

#if defined(__i386__) || defined(__x86_64__)  // we're targeting an x86 system

class x86_64Host : public Host {
 public:
  virtual int HardwareBreakpointCount() override;
  virtual int HardwareWatchpointCount() override;  // unusued in liberation
  virtual enum Platform Platform() const override { return Platform::x86_64; }
};

#else  // we're targeting something else (likely ARM)

#include "NOPHost.h"

using x86_64Host = NOPHost;

#endif /* _x86_64_ */
