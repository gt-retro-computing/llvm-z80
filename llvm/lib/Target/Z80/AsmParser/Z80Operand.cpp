//
// Created by codetector on 12/16/19.
//
#include <MCTargetDesc/Z80MCExpr.h>
#include "llvm/Support/Casting.h"
#include "Z80AsmParser.h"
#include "llvm/MC/MCInst.h"

#define DEBUG_TYPE "Z80-AsmParser"

using namespace llvm;
using namespace Z80AsmPrinter;

Z80Operand::Z80Operand(const Z80Operand &o) : MCParsedAsmOperand() {
  Kind = o.Kind;
  StartLoc = o.StartLoc;
  EndLoc = o.EndLoc;
  // TODO Copy Imm / Reg / Token depending on Kind
}

std::unique_ptr<Z80Operand>
Z80Operand::CreateReg(unsigned RegNo, SMLoc Start, SMLoc End) {
  auto newOperand = std::make_unique<Z80Operand>(k_Register, Start, End);
  newOperand->Reg.RegNumber = RegNo;
  // TODO Maybe Reg Type?
  return newOperand;
}

std::unique_ptr<Z80Operand>
Z80Operand::CreateToken(StringRef Token, SMLoc NameLoc) {
  auto TokOp = std::make_unique<Z80Operand>(k_Token, NameLoc, NameLoc);
  TokOp->Tok.Data = Token.data();
  TokOp->Tok.Length = Token.size();
  return TokOp;
}

std::unique_ptr<Z80Operand> Z80Operand::CreateImm(const MCExpr *Expr,
                                             SMLoc Start, SMLoc End) {
  auto Op = std::make_unique<Z80Operand>(k_Imm, Start, End);
  Op->Imm.Val = Expr;
  return Op;
}

std::unique_ptr<Z80Operand>
Z80Operand::CreateBrCC(const BrCC::BrCCTy ty, SMLoc Start, SMLoc End) {
  auto Op = std::make_unique<Z80Operand>(k_BrCC, Start, End);
  Op->BrCondCode.CC = ty;
  return Op;
}

void Z80Operand::addRegOperands(MCInst &Inst, unsigned N) const {
  assert(N == 1 && "Can not add more than 1 reg operand");
  Inst.addOperand(MCOperand::createReg(getReg()));
}

void Z80Operand::addImmOperands(MCInst &Inst, unsigned N) const {
  assert(N == 1 && "Invalid # of operands");
  addExpr(Inst, this->Imm.Val);
}

void Z80Operand::addExpr(MCInst &Inst, const MCExpr *expr) const {
  if (expr == nullptr) {
    // NULL
    LLVM_DEBUG(dbgs() << "addExpr() called with a null expr\n"
                         "adding an imm(0); \n");
    Inst.addOperand(MCOperand::createImm(0));
  } else if (const auto *CE = dyn_cast<MCConstantExpr>(expr)) {
    LLVM_DEBUG(dbgs() << "Collapsing const expr to imm\n");
    Inst.addOperand(MCOperand::createImm(CE->getValue()));
  } else {
    Inst.addOperand(MCOperand::createExpr(expr));
  }
}

StringRef Z80Operand::getToken() const {
  assert(Kind == k_Token && "Trying to get token from a non-token type.");
  return {Tok.Data, Tok.Length};
}

bool Z80Operand::isToken() const {
  return this->Kind == k_Token;
}

bool Z80Operand::isImm() const {
  return this->Kind == k_Imm;
}

bool Z80Operand::isReg() const {
  return this->Kind == k_Register;
}

unsigned int Z80Operand::getReg() const {
  return this->Reg.RegNumber;
}

bool Z80Operand::isMem() const {
  // TODO Z80Operand Mem Kind
  return false;
}

SMLoc Z80Operand::getStartLoc() const {
  return this->StartLoc;
}

SMLoc Z80Operand::getEndLoc() const {
  return this->EndLoc;
}

void Z80Operand::print(raw_ostream &OS) const {
}

void Z80Operand::addBranchCCOperand(MCInst &Inst, unsigned N) const {
  assert(N == 1);
  assert(this->isBranchCC() && "Adding a BR cc with a non-brcc operand");
//  Inst.addOperand(MCOperand::createImm(this->BrCondCode.CC));
  Inst.addOperand(MCOperand::createExpr(new Z80BrCCMCExpr(this->BrCondCode.CC)));
}

bool Z80Operand::isBranchCC() const {
  return this->Kind == k_BrCC;
  llvm_unreachable("Unimplemented");
}
