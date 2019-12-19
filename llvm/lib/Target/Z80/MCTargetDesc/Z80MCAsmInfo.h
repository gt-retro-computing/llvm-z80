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
}

#endif //LLVM_Z80ASMINFO_H
