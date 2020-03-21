//
// Created by codetector on 2/24/20.
//

#ifndef LLVM_Z80MCEXPR_H
#define LLVM_Z80MCEXPR_H

#include <llvm/MC/MCExpr.h>
#include "Z80MCAsmInfo.h"

using namespace llvm;

namespace llvm {
class Z80BrCCMCExpr: public MCTargetExpr {
public:
  explicit Z80BrCCMCExpr(uint8_t CC_Value);

  void anchor() override;

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;

  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;

  void visitUsedExpr(MCStreamer &Streamer) const override;

  MCFragment *findAssociatedFragment() const override;

  void fixELFSymbolsInTLSFixups(MCAssembler &assembler) const override;

private:
  Z80CondCode cc;

};
}

#endif //LLVM_Z80MCEXPR_H
