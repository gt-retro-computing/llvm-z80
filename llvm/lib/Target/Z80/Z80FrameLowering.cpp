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
    LLVM_DEBUG(dbgs() << "[TL45FrameLowering::determineFrameLayout] MF: '"
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

void llvm::Z80FrameLowering::emitPrologue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");

  bool hasFramePointer = hasFP(MF);

  MachineFrameInfo &MFI = MF.getFrameInfo();
  const Z80RegisterInfo *RI = STI.getRegisterInfo();
  const Z80InstrInfo *TII = STI.getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.begin();

  LLVM_DEBUG(dbgs() << "Emitting Prologue for function: " << MF.getName() << "\n");


  if (RI->needsStackRealignment(MF) && MFI.hasVarSizedObjects()) {
    report_fatal_error(
            "Z80 backend can't currently handle functions that need stack "
            "realignment and have variable sized objects");
  }

  if (MFI.hasVarSizedObjects()) {
    LLVM_DEBUG(dbgs() << "Detected Variable Sized Items on stack");
  }

  Register SPReg = Z80::SP;

  // Debug location must be unknown since the first debug location is used
  // to determine the end of the prologue.
  DebugLoc DL;

  determineFrameLayout(MF, hasFramePointer);

  // FIXME (note copied from Lanai): This appears to be overallocating.  Needs
  // investigation. Get the number of bytes to allocate from the FrameInfo.
  uint64_t StackSize = MFI.getStackSize();

  // Early exit if there is no need to allocate on the stack
  if (StackSize == 0 && !MFI.adjustsStack())
    return;



  // Allocate space on the stack if necessary.
  // aka Decrement stack pointer
  adjustReg(MBB, MBBI, DL, SPReg, SPReg, -StackSize, MachineInstr::FrameSetup);

  // Emit ".cfi_def_cfa_offset StackSize"
  unsigned CFIIndex = MF.addFrameInst(
          MCCFIInstruction::createDefCfaOffset(nullptr, -StackSize));
  BuildMI(MBB, MBBI, DL, TII->get(TargetOpcode::CFI_INSTRUCTION))
          .addCFIIndex(CFIIndex);

  // The frame pointer is callee-saved, and code has been generated for us to
  // save it to the stack. We need to skip over the storing of callee-saved
  // registers as the frame pointer must be modified after it has been saved
  // to the stack, not before.
  // FIXME: assumes exactly one instruction is used to save each callee-saved
  // register.
  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  std::advance(MBBI, CSI.size());

  // Iterate over list of callee-saved registers and emit .cfi_offset
  // directives.
  for (const auto &Entry : CSI) {
    int64_t Offset = MFI.getObjectOffset(Entry.getFrameIdx());
    Register Reg = Entry.getReg();
    unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::createOffset(
            nullptr, RI->getDwarfRegNum(Reg, true), Offset));
    BuildMI(MBB, MBBI, DL, TII->get(TargetOpcode::CFI_INSTRUCTION))
            .addCFIIndex(CFIIndex);
  }

  if (hasFramePointer) {
    llvm_unreachable("unable to process frame pointer yet");
  }
}

void llvm::Z80FrameLowering::emitEpilogue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
//TODO  llvm_unreachable("Z80::emitEpilogue");
}

bool llvm::Z80FrameLowering::hasFP(const llvm::MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();

  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken() ||
         TRI->needsStackRealignment(MF);
}
