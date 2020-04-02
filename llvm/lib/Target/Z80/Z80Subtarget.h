//
// Created by codetector on 12/14/19.
//

#ifndef LLVM_Z80SUBTARGET_H
#define LLVM_Z80SUBTARGET_H

#include "Z80.h"

#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "Z80InstrInfo.h"
#include "Z80FrameLowering.h"
#include "Z80ISelLowering.h"

#define GET_SUBTARGETINFO_HEADER
#include "Z80GenSubtargetInfo.inc"

namespace llvm {

class Z80Subtarget : public Z80GenSubtargetInfo {
private:
  virtual void anchor();

  Z80RegisterInfo RI;
  Z80InstrInfo InstrInfo;
  Z80TargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;
//	InstrItineraryData InstrItins;
  Z80FrameLowering FrameLowering;

public:
  Z80Subtarget(const Triple &TT, const StringRef CPU, const StringRef FS,
                const Z80TargetMachine &TM);
  const Z80InstrInfo *getInstrInfo() const override { return &InstrInfo; }

  const Z80RegisterInfo *getRegisterInfo() const override {
    return &RI;
  }

  const Z80TargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }

  const Z80FrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }

  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override;

};
}

#endif //LLVM_Z80SUBTARGET_H
