//
// Created by codetector on 3/21/20.
//
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetOptions.h"

#include "Z80FrameLowering.h"
#include "Z80Subtarget.h"

#define DEBUG_TYPE "z80-frame-lowering"


void Z80FrameLowering::determineFrameLayout(MachineFunction &MF, bool hasFP) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const Z80RegisterInfo *RI = STI.getRegisterInfo();

  // Get the number of bytes to allocate from the FrameInfo.
  uint64_t FrameSize = MFI.getStackSize();

  // Get the alignment.
  unsigned StackAlign = getStackAlignment();
  if (RI->needsStackRealignment(MF)) {
    unsigned MaxStackAlign = std::max(StackAlign, MFI.getMaxAlignment());
    FrameSize += (MaxStackAlign - StackAlign);
    StackAlign = MaxStackAlign;
  }

  // Set Max Call Frame Size
  uint64_t MaxCallSize = alignTo(MFI.getMaxCallFrameSize(), StackAlign);
  MFI.setMaxCallFrameSize(MaxCallSize);

  // Make sure the frame is aligned.
  FrameSize = alignTo(FrameSize, StackAlign);

  if (hasFP) {
    auto TMDL = MF.getDataLayout();
    uint64_t ptr_size = TMDL.getPointerSizeInBits() / 8;
    FrameSize += ptr_size;
    LLVM_DEBUG(dbgs() << "[Z80FrameLowering::determineFrameLayout] MF: '"
                      << MF.getName() << "' has frame pointer, incrementing stack frame size by ptr_size.\n");
  }

  // Update frame info.
  MFI.setStackSize(FrameSize);
}

void Z80FrameLowering::adjustReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MBBI,
                                  const DebugLoc &DL, Register DestReg,
                                  Register SrcReg, int64_t Val,
                                  MachineInstr::MIFlag Flag) const {
  MachineRegisterInfo &MRI = MBB.getParent()->getRegInfo();
  const Z80InstrInfo *TII = STI.getInstrInfo();
  const Z80RegisterInfo *RI = STI.getRegisterInfo();

  if (DestReg == SrcReg && Val == 0)
    return;

  if (DestReg != SrcReg) {
    llvm_unreachable("can't handle adjReg src!=dest yet");
  } else {
    if (SrcReg == Z80::SP) {
      // TODO Sketchy
      auto hlReg = Z80::HL;
      assert(isInt<16>(Val) && "Can't handle SPAdj more than 16bits");
      BuildMI(MBB, MBBI, DL, TII->get(Z80::LD16ri), hlReg).addImm(Val).setMIFlag(Flag);
      BuildMI(MBB, MBBI, DL, TII->get(Z80::ADD16HLr), hlReg).addReg(hlReg).addReg(SrcReg).setMIFlag(Flag);
      BuildMI(MBB, MBBI, DL, TII->get(Z80::LDSPHL), DestReg).addReg(hlReg).setMIFlag(Flag);
      return;
    }
    llvm_unreachable("Can't do yet lol.");
  }


  if (isInt<8>(Val)) {
    llvm_unreachable("lol didn't impl yet");
//    BuildMI(MBB, MBBI, DL, TII->get(TL45::ADDI), DestReg)
//            .addReg(SrcReg)
//            .addImm(Val)
//            .setMIFlag(Flag);
  } else if (isInt<16>(Val)) {
    llvm_unreachable("lol can't do yet");
  } else {
    report_fatal_error("adjustReg cannot yet handle adjustments >32 bits");
  }
}

/*
 * Here's a brief description of what this does
 *
 * [if hasFP]
 *  report_error (can't handle this yet)
 * [for each callee saved register PAIR]
 *  push %pair
 * [allocate space for locals]
 *  call adjustReg with the correct
 *
 *
 *
 */
void llvm::Z80FrameLowering::emitPrologue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");

  bool hasFramePointer = hasFP(MF);

  MachineFrameInfo &MFI = MF.getFrameInfo();
  const Z80RegisterInfo *RI = STI.getRegisterInfo();
  const Z80InstrInfo *TII = STI.getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  LLVM_DEBUG(dbgs() << "Emitting Prologue for function: " << MF.getName() << "\n");
  Register SPReg = STI.getTargetLowering()->getStackPointerRegisterToSaveRestore();
  assert(SPReg == Z80::SP && "SP is not sp???");

  if (RI->needsStackRealignment(MF) && MFI.hasVarSizedObjects()) {
    report_fatal_error(
            "Z80 backend can't currently handle functions that need stack "
            "realignment and have variable sized objects");
  }

  if (MFI.hasVarSizedObjects()) {
    LLVM_DEBUG(dbgs() << "Detected Variable Sized Items on stack");
  }

  if (hasFramePointer) {
    report_fatal_error("Z80Backend can't handle FP yet.");
  }

  // Debug location must be unknown since the first debug location is used
  // to determine the end of the prologue.
  DebugLoc DL;



}

void llvm::Z80FrameLowering::emitEpilogue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
//  bool hasFramePointer = hasFP(MF);
//
//  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
//  const Z80RegisterInfo *RI = STI.getRegisterInfo();
//  MachineFrameInfo &MFI = MF.getFrameInfo();
////  auto *RVFI = MF.getInfo<RISCVMachineFunctionInfo>();
//  DebugLoc DL = MBBI->getDebugLoc();
//  const Z80InstrInfo *TII = STI.getInstrInfo();
////  Register FPReg = TL45::bp;
//  Register SPReg = Z80::SP;
//
//  // Skip to before the restores of callee-saved registers
//  // FIXME: assumes exactly one instruction is used to restore each
//  // callee-saved register.
//  auto LastFrameDestroy = std::prev(MBBI, MFI.getCalleeSavedInfo().size());
//
//  uint64_t StackSize = MFI.getStackSize();
//
////  uint64_t FPOffset = StackSize;// - RVFI->getVarArgsSaveSize();
//
//  // Restore the stack pointer using the value of the frame pointer. Only
//  // necessary if the stack pointer was modified, meaning the stack size is
//  // unknown.
//  if (RI->needsStackRealignment(MF) || MFI.hasVarSizedObjects()) {
//    assert(hasFramePointer && "frame pointer should not have been eliminated");
////    adjustReg(MBB, LastFrameDestroy, DL, SPReg, FPReg, -FPOffset,
////              MachineInstr::FrameDestroy);
//    // TODO Fixme
//    BuildMI(MBB, LastFrameDestroy, DL, TII->get(TL45::ADDI)).addReg(SPReg).addReg(FPReg).addImm(-(StackSize - 4));
//  }
//
//  if (hasFramePointer) {
//
//    // BP is a linked list!!!
//    BuildMI(MBB, LastFrameDestroy, DL, TII->get(TL45::LW)).addReg(FPReg).addReg(FPReg).addImm(0);
//
//  }
}

bool llvm::Z80FrameLowering::hasFP(const llvm::MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();

  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken() ||
         TRI->needsStackRealignment(MF);
}
