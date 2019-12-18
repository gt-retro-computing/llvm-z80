//
// Created by codetector on 12/14/19.
//

#ifndef LLVM_Z80SUBTARGET_H
#define LLVM_Z80SUBTARGET_H

#include "Z80.h"

#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "Z80InstrInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "Z80GenSubtargetInfo.inc"

namespace llvm {
class Z80Subtarget : public Z80GenSubtargetInfo {

};
}

#endif //LLVM_Z80SUBTARGET_H
