//
// Created by codetector on 12/16/19.
//
#include "Z80AsmParser.h"
#include "llvm/MC/MCInst.h"

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

void Z80Operand::addRegOperands(MCInst &Inst, unsigned N) const {
  assert(N == 1 && "Can not add more than 1 reg operand");
  Inst.addOperand(MCOperand::createReg(getReg()));
}

void Z80Operand::addImmOperands(MCInst &Inst, unsigned N) const {
  assert(N == 1 && "Invalid # of operands");
}

void Z80Operand::addExpr(MCInst &, const MCExpr *expr) const {

}

StringRef Z80Operand::getToken() const {
  assert(Kind == k_Token && "Trying to get token from a non-token type.");
  return {Tok.Data, Tok.Length};
}

bool Z80Operand::isToken() const {
  return this->Kind == k_Token;
}

bool Z80Operand::isImm() const {
  return this->Kind == k_Imm8 || this->Kind == k_Imm16;
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
