#include "../src/Process.h"
#include <stdio.h>

std::string ProtToString(vm_prot_t prot) {
  std::string str = "";
  if (prot & VM_PROT_READ)
    str += "R";
  if (prot & VM_PROT_WRITE)
    str += "W";
  if (prot & VM_PROT_EXECUTE)
    str += "X";

  return str;
}

int main(int argc, char **argv) {
  if (Process::CanAttach()) {
    auto proc = Process::GetProcess(argv[1]);
    auto regions = proc->GetRegions(VM_PROT_ANY);
    for (auto &region : regions) {
      printf("Region 0x%lx-0x%lx\t%s\n", region.start, region.start + region.size,
             ProtToString(region.prot).data());
    }
  }
  else {
    printf("please sign %s with 'task_for_pid-allow' set to true\n", argv[0]);
  }
}
