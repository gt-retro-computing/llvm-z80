//
// Created by codetector on 3/21/20.
//

#ifndef LLVM_Z80FRAMELOWERING_H
#define LLVM_Z80FRAMELOWERING_H

#include "Z80.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class Z80Subtarget;

class Z80FrameLowering : public TargetFrameLowering {
protected:
  const Z80Subtarget &STI;
public:
  explicit Z80FrameLowering(const Z80Subtarget &sti, uint64_t Alignment)
          : TargetFrameLowering(StackGrowsDown, Alignment, 0, Alignment), STI(sti) {}
};
}

#endif //LLVM_Z80FRAMELOWERING_H
