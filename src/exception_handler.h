//
//  ExceptionHandler.h
//  Liberation
//
//  Created by satori
//  Copyright © 2016 satori. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>
#include "thread_state.h"

// TODO: be bothered to make getters for this
class Exception {
  friend class ExceptionHandler;

 public:
  Exception(mach_port_t exception_port, mach_port_t thread, mach_port_t task,
            exception_type_t type, exception_data_t code,
            mach_msg_type_number_t code_count)
  : exception_port_(exception_port),
    thread_(thread),
    task_(task),
    type_(type),
    code_(code),
    code_count_(code_count) {}

  // TODO: make member var assigned in ctor (also not a pointer)
  ThreadState *ExceptionThreadState();

 protected:
  mach_port_t exception_port_;
  mach_port_t thread_;
  mach_port_t task_;
  exception_type_t type_;
  exception_data_t code_;
  mach_msg_type_number_t code_count_;
};

class ExceptionHandler {
 public:
  static std::shared_ptr<ExceptionHandler> SharedHandler();

  bool SetupHandler();
  kern_return_t ExceptionCallback(Exception &);

 private:
  std::vector<Exception> exception_history_;
};
