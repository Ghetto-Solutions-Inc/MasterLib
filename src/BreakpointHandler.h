//
//  BreakpointHandler.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include "Breakpoint.h"
#include "Process.h"

#define BKPT_ENABLED (1 << 0)
#define BKPT_DISABLED (1 << 1)
#define BKPT_ALL (BKPT_ENABLED | BKPT_DISABLED)

class BreakpointHandler {
 public:
  BreakpointHandler() = default;
  static std::shared_ptr<BreakpointHandler> SharedHandler();

  bool InstallBreakpoint(Breakpoint *bp, bool immActive = true);
  bool UninstallBreakpoint(Breakpoint *bp);
  bool DisableBreakpoint(Breakpoint *bp);
  bool EnableBreakpoint(Breakpoint *bp);

  Breakpoint *BreakpointAtAddress(vm_address_t address);

  std::vector<Breakpoint *> Breakpoints(uint32_t flags = BKPT_ALL);

 private:
  std::vector<Breakpoint *> breakpoints_;  // TODO: switch to smart pointer?
};
