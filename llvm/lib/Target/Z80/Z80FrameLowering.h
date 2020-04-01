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
          : TargetFrameLowering(StackDirection::StackGrowsDown, Align(Alignment), 0, Align(Alignment)), STI(sti) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;

  void adjustReg(MachineBasicBlock &MBB,
                 MachineBasicBlock::iterator MBBI,
                 const DebugLoc &DL, Register DestReg,
                 Register SrcReg, int64_t Val,
                 MachineInstr::MIFlag Flag) const;
private:
  void determineFrameLayout(MachineFunction &MF, bool hasFP) const;

  };
}

#endif //LLVM_Z80FRAMELOWERING_H
