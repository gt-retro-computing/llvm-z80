//
// Created by codetector on 12/14/19.
//

#include "Z80.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target llvm::TheZ80Target;

extern "C" void LLVMInitializeZ80TargetInfo() {
  RegisterTarget<Triple::Z80> X(TheZ80Target, "z80", "Z80", "Z80");
}
