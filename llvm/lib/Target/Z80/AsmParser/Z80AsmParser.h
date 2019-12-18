//
// Created by codetector on 12/16/19.
//

#ifndef LLVM_Z80ASMPARSER_H
#define LLVM_Z80ASMPARSER_H

#include "llvm/MC/MCParser/MCTargetAsmParser.h"

using namespace llvm;
namespace {
struct Z80Operand;

class Z80AsmParser : public MCTargetAsmParser {
public:
  Z80AsmParser(const MCSubtargetInfo &_STI, MCAsmParser &_Parser,
                const MCInstrInfo &MII, const MCTargetOptions &Options)
          : MCTargetAsmParser(Options, _STI, MII), STI(_STI), Parser(_Parser) {
//            setAvailableFeatures(ComputeAvailableFeatures(STI.getFeatureBits()));
    MCAsmParserExtension::Initialize(Parser);
  }
  bool ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;
private:
  const MCSubtargetInfo &STI;
  MCAsmParser &Parser;
  bool ParseRegister(unsigned &RegNo, OperandVector &Operands);
};

}
#endif //LLVM_Z80ASMPARSER_H
