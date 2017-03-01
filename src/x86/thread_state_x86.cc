//
//  thread_state_x86.h
//  MasterLib
//
//  Created by satori
//  Copyright © 2017 satori. All rights reserved.
//

#include <sstream>
#include "thread_state_x86.h"

bool x86ThreadState::Load() {
    x86_thread_state32_t &state = thread_state_;
    mach_msg_type_number_t count = x86_THREAD_STATE32_COUNT;
    thread_get_state(thread_, x86_THREAD_STATE32, (thread_state_t)&state, &count);
    
    count = x86_DEBUG_STATE32_COUNT;
    thread_get_state(thread_, x86_DEBUG_STATE32, (thread_state_t)&debug_state_, &count);
    
    uint32_t *statePtr = (uint32_t *)&state;
    STATE_ADD_REGISTER(this, "EAX", statePtr + 0);
    STATE_ADD_REGISTER(this, "EBX", statePtr + 1);
    STATE_ADD_REGISTER(this, "ECX", statePtr + 2);
    STATE_ADD_REGISTER(this, "EDX", statePtr + 3);
    STATE_ADD_REGISTER(this, "EDI", statePtr + 4);
    STATE_ADD_REGISTER(this, "ESI", statePtr + 5);
    STATE_ADD_REGISTER(this, "EBP", statePtr + 6);
    STATE_ADD_REGISTER(this, "ESP", statePtr + 7);
    STATE_ADD_REGISTER(this, "SS", statePtr + 8);
    STATE_ADD_REGISTER(this, "EFLAGS", statePtr + 9);
    STATE_ADD_REGISTER(this, "EIP", statePtr + 10);
    STATE_ADD_REGISTER(this, "CS", statePtr + 11);
    STATE_ADD_REGISTER(this, "DS", statePtr + 12);
    STATE_ADD_REGISTER(this, "ES", statePtr + 13);
    STATE_ADD_REGISTER(this, "FS", statePtr + 14);
    STATE_ADD_REGISTER(this, "GS", statePtr + 15);
    
    return true;
}

bool x86ThreadState::Save() {
    thread_set_state(thread_, x86_THREAD_STATE32, (thread_state_t)&thread_state_, x86_THREAD_STATE32_COUNT);
    
    thread_set_state(thread_, x86_DEBUG_STATE32, (thread_state_t)&debug_state_, x86_DEBUG_STATE32_COUNT);
    return true;

}

std::string x86ThreadState::Description() {
    std::ostringstream stream;
    
    for (auto &reg : registers_) {
        uint64_t val = reg.Value<uint32_t>();
        stream << reg.Name() << ": " << std::dec << val << " [" << std::hex << val << "]" << std::endl;
    }
    return stream.str();
}

vm_address_t x86ThreadState::CurrentAddress() {
    return thread_state_.__eip;
}
