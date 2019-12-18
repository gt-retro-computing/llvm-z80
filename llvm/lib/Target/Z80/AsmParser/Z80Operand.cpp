//
// Created by codetector on 12/16/19.
//
#include "Z80AsmParser.h"

using namespace llvm;

struct Z80Operand : MCParsedAsmOperand {
  enum KindTy {
    k_Token,
    k_Register,
    k_Imm8,
    k_Imm16
  } Kind;
  SMLoc StartLoc, EndLoc;

  enum RegTypeTy {
    k_Reg8,
    k_RegPair16
  };

  struct Token {
    const char* Data;
    unsigned Length;
  };

  struct RegOp {
    RegTypeTy RegType;
    uint8_t RegNumber;
  };

  struct ImmOp {
    const MCExpr *Val;
  };

  union {
    struct Token Tok;
    struct RegOp Reg;
    struct ImmOp Imm;
  };

  Z80Operand(KindTy K) : MCParsedAsmOperand(), Kind(K){}
};
