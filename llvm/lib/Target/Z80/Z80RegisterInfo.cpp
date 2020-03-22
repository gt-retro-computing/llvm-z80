//
// Created by codetector on 12/16/19.
//

#include "MCTargetDesc/Z80MCTargetDesc.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/Support/Debug.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "Z80RegisterInfo.h"
#include "Z80FrameLowering.h"

#define DEBUG_TYPE "Z80-RegisterInfo"
#define GET_REGINFO_TARGET_DESC
#include "Z80GenRegisterInfo.inc"

using namespace llvm;

Z80RegisterInfo::Z80RegisterInfo() : Z80GenRegisterInfo(0,0,0,0) {}

const uint32_t *Z80RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                                      CallingConv::ID id) const {
  return CC_Save_RegMask;
}

const MCPhysReg *
Z80RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CC_Save_SaveList;
}

BitVector Z80RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  auto vec = BitVector(getNumRegs());

  vec.set(Z80::SP);
  vec.set(Z80::flags);

  return vec;
}

void
Z80RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                                     unsigned FIOperandNum,
                                     RegScavenger *RS) const {

}

Register Z80RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Z80::SP;
}

