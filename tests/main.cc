#include "../src/disassembler.h"
#include <iostream>

//std::string ProtToString(vm_prot_t prot) {
//  std::string str = "";
//  if (prot & VM_PROT_READ)
//    str += "R";
//  if (prot & VM_PROT_WRITE)
//    str += "W";
//  if (prot & VM_PROT_EXECUTE)
//    str += "X";
//
//  return str;
//}

//int main(int argc, char **argv) {
//    x86ThreadState *state = new x86ThreadState(mach_thread_self());
//    state->Load();
//    
//    std::cout << state->Description() << std::endl;
//}


const uint8_t code[] = { 0x00, 0x00, 0xA0, 0xE3 };

int main() {
    Disassembler ds = Disassembler(0x0, code, 4, Arch::ARM, Mode::ModeARM);
    ds.Setup();
    auto instr = ds.Disassemble();
    std::cout << instr.mnemonic() << " " << instr.operands() << std::endl;
}
