//
// Created by codetector on 2/24/20.
//

#include "Z80MCExpr.h"
#include "llvm/Support/raw_ostream.h"

// Z80BrCCMCexpr

void Z80BrCCMCExpr::anchor() {}

void Z80BrCCMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  switch (this->cc) {
    case Z80CondCode ::NZ:
      OS << "nz"; break;
    case Z80CondCode ::Z:
      OS << "z"; break;
    case Z80CondCode ::C:
      OS << "c"; break;
    case Z80CondCode ::NC:
      OS << "nc"; break;
    case Z80CondCode ::M:
      OS << "m"; break;
    case Z80CondCode ::P:
      OS << "p"; break;
    case Z80CondCode ::PE:
      OS << "pe"; break;
    case Z80CondCode ::PO:
      OS << "po"; break;
  }
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

Z80BrCCMCExpr::Z80BrCCMCExpr(uint8_t ccValue): MCTargetExpr() {
  assert(isUInt<3>(ccValue) && "CC Value is beyond possible ranges");
  this->cc = static_cast<Z80CondCode>(ccValue);
}
