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
#include "Z80InstrInfo.h"

using namespace llvm;

Z80RegisterInfo::Z80RegisterInfo() : Z80GenRegisterInfo(0, 0, 0, 0) {}

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


/// This method must be overriden to eliminate abstract frame indices from
/// instructions which may use them. The instruction referenced by the
/// iterator contains an MO_FrameIndex operand which must be eliminated by
/// this method. This method may modify or replace the specified instruction,
/// as long as it keeps the iterator pointing at the finished product.
/// SPAdj is the SP adjustment due to call frame setup instruction.
/// FIOperandNum is the FI operand number.
void
Z80RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MII, int SPAdj,
                                     unsigned FIOperandNum,
                                     RegScavenger *RS) const {

  MachineInstr &MI = *MII;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineBasicBlock &MBB = *MI.getParent();

  LLVM_DEBUG(dbgs() << "\nFunction : " << MF.getName() << "\n";
                     dbgs() << "<--------->\n"
                            << MI);

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  uint64_t StackSize = MF.getFrameInfo().getStackSize();
  int64_t SPOffset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  LLVM_DEBUG(dbgs() << "FrameIndex : " << FrameIndex << "\n"
                    << "spOffset   : " << SPOffset << "\n"
                    << "stackSize  : " << StackSize << "\n"
                    << "alignment  : "
                    << MF.getFrameInfo().getObjectAlignment(FrameIndex)
                    << "\n");

  unsigned OpNo = FIOperandNum;

  MachineFrameInfo &MFI = MF.getFrameInfo();

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  int MinCSFI = 0;
  int MaxCSFI = -1;

  if (CSI.size()) {
    MinCSFI = CSI[0].getFrameIdx();
    MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
  }

  // The following stack frame objects are always
  // referenced relative to $sp:
  //  1. Outgoing arguments.
  //  2. Pointer to dynamically allocated stack space.
  //  3. Locations for callee-saved registers.
  // Everything else is referenced relative to whatever register
  // getFrameRegister() returns.
  unsigned FrameReg;

  FrameReg = Z80::SP;
// TODO not sure precisely what this code is supposed to do and in its current state,
// causes bugs when working with the stack.

//  if (FrameIndex >= MinCSFI && FrameIndex <= MaxCSFI)
//    FrameReg = TL45::sp;
//  else {
//    const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
//    if (TFI->hasFP(MF)) {
//      FrameReg = TL45::bp;
//    }
//    else {
// //      if ((MI.getNumOperands()> OpNo+2) && MI.getOperand(OpNo+2).isReg() && MI.getOperand(OpNo+2).getReg().id() != TL45::eflags)
// //        FrameReg = MI.getOperand(OpNo+2).getReg();
// //      else
//        FrameReg = TL45::sp;
//    }
//  }
  // Calculate final offset.
  // - There is no need to change the offset if the frame object
  //   is one of the
  //   following: an outgoing argument, pointer to a dynamically allocated
  //   stack space or a $gp restore location,
  // - If the frame object is any of the following,
  //   its offset must be adjusted
  //   by adding the size of the stack:
  //   incoming argument, callee-saved register location or local variable.
  int64_t Offset;
  bool IsKill = false;
  Offset = SPOffset + (int64_t) StackSize;

  LLVM_DEBUG(dbgs() << "Offset     : " << Offset << "\n"
                    << "<--------->\n");

  const Z80InstrInfo &TII = *static_cast<const Z80InstrInfo *>(MF.getSubtarget().getInstrInfo());
//  if (!MI.isDebugValue() &&
//      !Z80InstrInfo::validImmediate(MI.getOpcode(), FrameReg, Offset)) {
//    MachineBasicBlock &MBB = *MI.getParent();
//    DebugLoc DL = II->getDebugLoc();
//    unsigned NewImm;
//    FrameReg = TII.loadImmediate(FrameReg, Offset, MBB, II, DL, NewImm);
//    Offset = SignExtend64<16>(NewImm);
////    IsKill = true;
//  }
  MI.getOperand(OpNo).ChangeToImmediate(Offset);
  assert(MI.getOpcode() == Z80::LD16ri && MI.getOperand(0).isReg() &&
         MI.getOperand(0).getReg() == Z80::HL && "Constraints: EliminateFI");
  std::advance(MII, 1);
  BuildMI(MBB, MII, MI.getDebugLoc(), TII.get(Z80::ADD16HLr),
          MI.getOperand(0).getReg())
          .addReg(MI.getOperand(0).getReg()).addReg(FrameReg);
}

Register Z80RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Z80::SP;
}

