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

#define DEBUG_TYPE "Z80-AsmParser"

using namespace llvm;
using namespace Z80AsmPrinter;

static unsigned
MatchRegisterName(StringRef Name); // Body is generate by tableGen
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

bool
Z80AsmParser::ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) {
  SmallVector<std::unique_ptr<MCParsedAsmOperand>, 1> Operands;
  if (ParseRegister(RegNo, Operands))
    return true;

//    TL45Operand &Operand = static_cast<TL45Operand &>(*Operands.front());
//    StartLoc = Operand.getStartLoc();
//    EndLoc = Operand.getEndLoc();
  return false;
}

bool Z80AsmParser::ParseImmediate(OperandVector &Operands) {
  SMLoc S = Parser.getTok().getLoc();
  SMLoc E = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);

  const MCExpr *Expr = nullptr;
  AsmToken::TokenKind kind = getLexer().getKind();
  switch (kind) {
    default:
      return true;
    case AsmToken::Minus:
    case AsmToken::Integer:
      if (getParser().parseExpression(Expr))
        return true;

      Operands.push_back(Z80Operand::CreateImm(Expr, S, E));
      return false;
  }
}

bool Z80AsmParser::ParseSymbolReference(OperandVector &Operands) {
  SMLoc S = Parser.getTok().getLoc();
  StringRef Identifier;
  if (Parser.parseIdentifier(Identifier))
    return true;

  SMLoc E = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);

  // TODO relocations?

  // Parse a symbol
  MCSymbol *Sym = getContext().getOrCreateSymbol(Identifier);
  const MCExpr *Res = MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None,
                                              getContext());
  Operands.push_back(Z80Operand::CreateImm(Res, S, E));
  return false;
}

/*
 * return - false: parse success, true: failed to parse
 */
bool Z80AsmParser::ParseOperand(OperandVector &Operands) {
  // A register operand is always alone
  auto CurrentTokenKind = getLexer().getKind();
  if (CurrentTokenKind == AsmToken::LParen) {
    AsmToken LParn = getLexer().getTok();
    auto LParnPushback = getLexer().Lex(); // Consume 1 token,
    // but unlex will use this
    Operands.push_back(
            Z80Operand::CreateToken(LParn.getString(), LParn.getLoc()));
    if (!ParseOperand(Operands)) {
      // Success
      auto RParn = getLexer().getTok();
      getLexer().Lex();
      Operands.push_back(
              Z80Operand::CreateToken(RParn.getString(), RParn.getLoc()));
      return false;
    } else {
      // Fail & backtrack
      Operands.pop_back();
      getLexer().UnLex(LParnPushback);
      return true;
    }
  }

  unsigned RegNo = 0;
  if (!ParseRegister(RegNo, Operands)) {
    return false; // Register Parse Success
  }

  // An immediate or expression operand can be alone
//  SMLoc S = getLexer().getTok().getLoc();
  if (!ParseImmediate(Operands) || !ParseSymbolReference(Operands)) {
    return false;
  }
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
    auto CurrentTokenKind = getLexer().getKind();
    if (CurrentTokenKind == AsmToken::Comma) {
      getLexer().Lex();
      continue;
    }

    // Parse next operand
    if (ParseOperand(Operands))
      return true;
  } // End While

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
                                           MCStreamer &Out,
                                           uint64_t &ErrorInfo,
                                           bool MatchingInlineAsm) {
  MCInst Inst;

  if (MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm)) {
    Out.EmitInstruction(Inst, this->STI);
    return false; // Success
  }
  return Error(IDLoc, "Failed to parse instruction");
}

OperandMatchResultTy Z80AsmParser::parseBranchCC(OperandVector &) {
  return MatchOperand_Success;
}



extern "C" void LLVMInitializeZ80AsmParser() {
  RegisterMCAsmParser<Z80AsmParser> X(TheZ80Target);
}
