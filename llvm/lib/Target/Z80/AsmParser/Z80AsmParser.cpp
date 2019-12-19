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
using namespace Z80AsmPrinter;

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
      const StringRef regNameLowerCase = regName.lower();
      RegNo = MatchRegisterName(regNameLowerCase);
      if (RegNo != 0) {
        getLexer().Lex();
        Operands.push_back(Z80Operand::CreateReg(RegNo, S, E));
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

/*
 * return - false: parse success, true: failed to parse
 */
bool Z80AsmParser::ParseOperand(OperandVector &Operands) {
  // A register operand is always alone
  switch (getLexer().getKind()) {
    // If any special shit need to be processed
    default:
      break;
  }
  unsigned RegNo = 0;
  if (!ParseRegister(RegNo, Operands)) {
    return false; // Register Parse Success
  }

  // TODO parse immediate

  return true;
}

bool Z80AsmParser::ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                                    SMLoc NameLoc, OperandVector &Operands) {
  // First operand is token for instruction
  Operands.push_back(Z80Operand::CreateToken(Name, NameLoc));

  // If there are no more operands, then finish
  if (getLexer().is(AsmToken::EndOfStatement))
    return false;

  // Parse first operand
  if (ParseOperand(Operands))
    return true;

  // Parse until end of statement, consuming commas between operands
  while (getLexer().isNot(AsmToken::EndOfStatement)) {
    // Consume comma token
    if (getLexer().is(AsmToken::Comma)) {
      getLexer().Lex();
      continue;
    }

    // Parse next operand
    if (ParseOperand(Operands))
      return true;
  }

  if (getLexer().isNot(AsmToken::EndOfStatement))
    return Error(getLexer().getTok().getLoc(),
                 "unexpected token in operand list");

  return false;
}

bool Z80AsmParser::ParseDirective(AsmToken DirectiveID) {
  return false;
}

bool Z80AsmParser::MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                                           OperandVector &Operands,
                                           MCStreamer &Out, uint64_t &ErrorInfo,
                                           bool MatchingInlineAsm) {
  MCInst Inst;

  if (MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm)) {
    Out.EmitInstruction(Inst, this->STI);
    return false; // Success
  }
  return Error(IDLoc, "Failed to parse instruction");
}

extern "C" void LLVMInitializeZ80AsmParser() {
  RegisterMCAsmParser<Z80AsmParser> X(TheZ80Target);
}
