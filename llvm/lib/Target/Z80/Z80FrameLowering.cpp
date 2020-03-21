//
// Created by codetector on 3/21/20.
//

#include "Z80FrameLowering.h"

void llvm::Z80FrameLowering::emitPrologue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  llvm_unreachable("Z80::emitPrologue");
}

void llvm::Z80FrameLowering::emitEpilogue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  llvm_unreachable("Z80::emitEpilogue");
}

bool llvm::Z80FrameLowering::hasFP(const llvm::MachineFunction &MF) const {
  return false;
}
