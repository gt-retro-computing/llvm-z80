//
// Created by codetector on 12/18/19.
//

#ifndef LLVM_Z80ASMINFO_H
#define LLVM_Z80ASMINFO_H

#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class StringRef;
class Target;
class Z80MCAsmInfo : public MCAsmInfoELF {
  void anchor() override {}
public:
  explicit Z80MCAsmInfo(const Triple &TT);
};
enum Z80CondCode {
    NZ = 0b000,
    Z = 0b001,
    NC = 0b010,
    C = 0b011,
    PO = 0b100,
    PE = 0b101,
    P = 0b110,
    M = 0b111
};
}

#endif //LLVM_Z80ASMINFO_H
