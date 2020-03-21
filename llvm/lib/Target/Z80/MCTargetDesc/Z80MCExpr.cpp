//
// Created by codetector on 2/24/20.
//

#include "Z80MCExpr.h"

// Z80BrCCMCexpr

void Z80BrCCMCExpr::anchor() {}

void Z80BrCCMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
}

bool Z80BrCCMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                              const MCAsmLayout *Layout,
                                              const MCFixup *Fixup) const {
  return false;
}

void Z80BrCCMCExpr::visitUsedExpr(MCStreamer &Streamer) const {

}

MCFragment *Z80BrCCMCExpr::findAssociatedFragment() const {
  return nullptr;
}

void Z80BrCCMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &assembler) const {
}
