

#pragma once

#include "Host.h"

#if defined(__arm__) || defined(__arm64__)

class ARMv7Host : public Host {
 public:
  virtual int HardwareBreakpointCount() override;
  virtual int HardwareWatchpointCount() override;  // unusued in liberation
  virtual enum Platform Platform() const { return Platform::ARMv7; }
};

#else

#include "NOPHost.h"

using ARMv7Host = NOPHost;

#endif
