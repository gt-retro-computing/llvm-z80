//
// Created by codetector on 12/16/19.
//

#ifndef LLVM_Z80INSTRINFO_H
#define LLVM_Z80INSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"
#include "Z80RegisterInfo.h"
#define GET_INSTRINFO_HEADER
#include "Z80GenInstrInfo.inc"

namespace llvm {
class Z80InstrInfo : public Z80GenInstrInfo {
  const Z80RegisterInfo RI;
public:
  Z80InstrInfo();

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;
};
}

#endif //LLVM_Z80INSTRINFO_H
