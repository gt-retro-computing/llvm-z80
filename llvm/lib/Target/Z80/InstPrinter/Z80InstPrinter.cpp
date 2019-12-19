//
// Created by codetector on 12/14/19.
//

// LLVM -debug-only
#define DEBUG_TYPE "Z80-InstPrinter"

#include "Z80InstPrinter.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//#define PRINT_ALIAS_INSTR
#include "Z80GenAsmWriter.inc"

void Z80InstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << StringRef(getRegisterName(RegNo)).lower();
}

void Z80InstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                StringRef Annot, const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

namespace Z80 {
static void printExpr(const MCExpr *Expr, raw_ostream &OS) {

  const MCSymbolRefExpr *SRE;
  const MCBinaryExpr *BinExp;
  switch (Expr->getKind()) {
    case MCExpr::ExprKind::SymbolRef:
      SRE = dyn_cast<MCSymbolRefExpr>(Expr);
      assert(SRE && "Unexpected MCExpr type.");
      OS << SRE->getSymbol();
      break;
    case MCExpr::ExprKind ::Binary:
      BinExp = dyn_cast<MCBinaryExpr>(Expr);
      assert(BinExp && "Failed to cast a binexp to MCBinaryExpr");
      printExpr(BinExp->getLHS(), OS);
      switch (BinExp -> getOpcode()) {
        case MCBinaryExpr::Opcode ::Add: OS << " + "; break;
        case MCBinaryExpr::Opcode ::Sub: OS << " - "; break;
        default: llvm_unreachable("unsupported operation");
      }
      printExpr(BinExp->getRHS(), OS);
      break;
    case MCExpr::ExprKind ::Constant:
      OS << dyn_cast<MCConstantExpr>(Expr)->getValue();
      break;
    default:
      Expr->dump();
      llvm_unreachable("Can't print this MCExpr");
  }
//  if SRE->getSymbol().
}
}

void Z80InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Z80::printExpr(Op.getExpr(), O);
}

bool Z80InstPrinter::applyTargetSpecificCLOption(StringRef Opt) {
  return true;
}

