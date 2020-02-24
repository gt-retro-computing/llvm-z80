//
// Created by codetector on 12/16/19.
//

#ifndef LLVM_Z80ASMPARSER_H
#define LLVM_Z80ASMPARSER_H

#include "llvm/MC/MCParser/MCTargetAsmParser.h"

using namespace llvm;
namespace Z80AsmPrinter {
struct Z80Operand;

struct Z80Operand : MCParsedAsmOperand {
public:
  enum KindTy {
    k_Token,
    k_Register,
    k_Imm,
    k_BrCC
  } Kind;
  SMLoc StartLoc, EndLoc;


  struct Token {
    const char *Data;
    unsigned Length;
  };

  struct RegOp {
//    enum RegTypeTy {
//      k_Reg8,
//      k_RegPair16
//    } RegType;
    unsigned RegNumber;
  };

  struct ImmOp {
    const MCExpr *Val;
  };

  union {
    struct Token Tok;
    struct RegOp Reg;
    struct ImmOp Imm;
  };

  void addExpr(MCInst &, const MCExpr *expr) const;

  void addRegOperands(MCInst &Inst, unsigned N) const;

  void addImmOperands(MCInst &Inst, unsigned N) const;

  void addBranchCCOperand(MCInst &Inst, unsigned N) const;

  bool isToken() const override;

  bool isImm() const override;

  bool isReg() const override;

  bool isBranchCC() const;

  unsigned int getReg() const override;

  StringRef getToken() const;

  bool isMem() const override;

  SMLoc getStartLoc() const override;

  SMLoc getEndLoc() const override;

  void print(raw_ostream &OS) const override;

  Z80Operand(KindTy K) : MCParsedAsmOperand(), Kind(K) {}

  Z80Operand(KindTy K, SMLoc Start, SMLoc End) : MCParsedAsmOperand(),
                                                 Kind(K), StartLoc(Start),
                                                 EndLoc(End) {}

  Z80Operand(const Z80Operand &o);

  // Factory methods
  static std::unique_ptr<Z80Operand> CreateReg(unsigned RegNo, SMLoc Start,
                                               SMLoc End);

  static std::unique_ptr<Z80Operand> CreateToken(StringRef Token,
                                                 SMLoc NameLoc);

  static std::unique_ptr<Z80Operand> CreateImm(const MCExpr*, SMLoc, SMLoc);
};

class Z80AsmParser : public MCTargetAsmParser {
#define GET_ASSEMBLER_HEADER

#include "Z80GenAsmMatcher.inc"

public:
  Z80AsmParser(const MCSubtargetInfo &STI, MCAsmParser &_Parser,
               const MCInstrInfo &MII, const MCTargetOptions &Options)
          : MCTargetAsmParser(Options, STI, MII), STI(STI), Parser(_Parser) {
//            setAvailableFeatures(ComputeAvailableFeatures(STI.getFeatureBits()));
    MCAsmParserExtension::Initialize(Parser);
  }

  bool ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;

  bool ParseImmediate(OperandVector &Operands);

  bool ParseSymbolReference(OperandVector &Operands);

  bool ParseOperand(OperandVector &Operands);

  bool
  ParseInstruction(ParseInstructionInfo &Info, StringRef Name, SMLoc NameLoc,
                   OperandVector &Operands) override;

  bool ParseDirective(AsmToken DirectiveID) override;

  bool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

private:
  const MCSubtargetInfo &STI;
  MCAsmParser &Parser;

  // Private Helper Function
  bool ParseRegister(unsigned &RegNo, OperandVector &Operands);

  // Matchers
  OperandMatchResultTy parseBranchCC(OperandVector &);
};

}
#endif //LLVM_Z80ASMPARSER_H
