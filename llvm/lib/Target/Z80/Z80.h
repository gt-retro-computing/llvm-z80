//
// Created by codetector on 12/14/19.
//

#ifndef LLVM_Z80_H
#define LLVM_Z80_H

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/Target/TargetMachine.h"
#include "MCTargetDesc/Z80MCTargetDesc.h"

namespace llvm {
class TargetMachine;
class Z80TargetMachine;
class MCInst;
FunctionPass *createZ80ISelDag(Z80TargetMachine &TM, CodeGenOpt::Level OptLevel);
bool LowerZ80MachineOperandToMCOperand(const MachineOperand &MO,
                                        MCOperand &MCOp,
                                        const AsmPrinter &AP);
}

#endif //LLVM_Z80_H
