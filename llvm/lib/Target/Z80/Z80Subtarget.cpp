//
// Created by codetector on 12/14/19.
//

#include "Z80Subtarget.h"
#include "Z80.h"
#include "llvm/Support/TargetRegistry.h"

#define DEBUG_TYPE "tl45-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "Z80GenSubtargetInfo.inc"

using namespace llvm;

Z80Subtarget::Z80Subtarget(const Triple &TT, const StringRef CPU, const StringRef FS, const Z80TargetMachine &TM) :
        Z80GenSubtargetInfo(TT, CPU, FS),
        TLInfo(TM, *this),
        FrameLowering(*this, /* Alignment in units of memory */ 4) {}

const SelectionDAGTargetInfo *Z80Subtarget::getSelectionDAGInfo() const {
  return &TSInfo;
}

void Z80Subtarget::anchor() {

}
