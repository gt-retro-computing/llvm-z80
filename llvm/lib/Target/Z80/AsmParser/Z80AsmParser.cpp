//
// Created by codetector on 12/14/19.
//
#include "Z80AsmParser.h"
#include "llvm/ADT/STLExtras.h"
#include "MCTargetDesc/Z80MCTargetDesc.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/TargetRegistry.h"

#define DEBUG_TYPE "Z80-asm-parser"

using namespace llvm;

static unsigned MatchRegisterName(StringRef Name); // Body is generate by tableGen
#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "Z80GenAsmMatcher.inc"

bool Z80AsmParser::ParseRegister(unsigned &RegNo, OperandVector &Operands) {
  SMLoc S = Parser.getTok().getLoc();
  SMLoc E = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);

  switch (getLexer().getKind()) {
    default:
      return true;
    case AsmToken::Identifier:
      const StringRef regName = getLexer().getTok().getIdentifier();
      RegNo = MatchRegisterName(regName);
      if (RegNo != 0) {
        getLexer().Lex();
        Operands.push_back(Z80Operand::createReg(RegNo, S, E));
        return false;
      }

      return true;
  }
}
bool Z80AsmParser::ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) {
  SmallVector<std::unique_ptr<MCParsedAsmOperand>, 1> Operands;
  if (ParseRegister(RegNo, Operands))
    return true;

//    TL45Operand &Operand = static_cast<TL45Operand &>(*Operands.front());
//    StartLoc = Operand.getStartLoc();
//    EndLoc = Operand.getEndLoc();
  return false;
}

extern "C" void LLVMInitializeZ80AsmParser() {
  RegisterMCAsmParser<Z80AsmParser> X(TheZ80Target);
}
