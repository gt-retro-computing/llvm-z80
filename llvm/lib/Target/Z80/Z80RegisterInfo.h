//
// Created by codetector on 12/16/19.
//

#ifndef LLVM_Z80REGISTERINFO_H
#define LLVM_Z80REGISTERINFO_H

#define GET_REGINFO_HEADER
#include "Z80GenRegisterInfo.inc"

namespace llvm {

struct Z80RegisterInfo : public Z80GenRegisterInfo {

  Z80RegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID id) const override;
};
}

#endif //LLVM_Z80REGISTERINFO_H
