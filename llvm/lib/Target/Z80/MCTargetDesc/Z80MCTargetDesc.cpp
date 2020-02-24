//
// Created by codetector on 12/14/19.
//
#include "Z80InstPrinter.h"
#include "Z80MCTargetDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC

#include "Z80GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC

#include "Z80GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC

#include "Z80GenRegisterInfo.inc"
#include "Z80MCAsmInfo.h"

using namespace llvm;


static MCRegisterInfo *createZ80MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitZ80MCRegisterInfo(X, 0);
  return X;
}

static MCAsmInfo *createZ80MCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT,
                                     const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new Z80MCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createZ80MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitZ80MCInstrInfo(X);
  return X;
}

static MCInstPrinter *createZ80MCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  // TODO: Lifetime issue? maybe?
  return new Z80InstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeZ80TargetMC() {
  // TODO Register Target

  // Register MCRegInfo
  TargetRegistry::RegisterMCRegInfo(TheZ80Target, createZ80MCRegisterInfo);

  // Register MCAsmInfo
  TargetRegistry::RegisterMCAsmInfo(TheZ80Target, createZ80MCAsmInfo);

  // Register MCInstInfo
  TargetRegistry::RegisterMCInstrInfo(TheZ80Target, createZ80MCInstrInfo);

  // Register MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(TheZ80Target, createZ80MCInstPrinter);
}
