
//
// Created by codetector on 9/7/19.
//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_Z80_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_Z80_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY Z80TargetInfo : public TargetInfo {
  static const TargetInfo::GCCRegAlias GCCRegAliases[];
  static const char *const GCCRegNames[];
//  static const Builtin::Info BuiltinInfo[];
public:
  Z80TargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts) : TargetInfo(Triple) {
    TLSSupported = false;
    PointerWidth = 16;
    PointerAlign = 8;
    IntWidth = 16;
    IntAlign = 8;
    LongWidth = 32;
    LongAlign = 8;
    LongLongWidth = 64;
    LongLongAlign = 8;
    SuitableAlign = 8;
    DefaultAlignForAttributeAligned = 8;
//    HalfWidth = 16;
//    HalfAlign = 8;
//    FloatWidth = 32;
//    FloatAlign = 8;
//    DoubleWidth = 32;
//    DoubleAlign = 8;
//    DoubleFormat = &llvm::APFloat::IEEEsingle();
//    LongDoubleWidth = 32;
//    LongDoubleAlign = 8;
//    LongDoubleFormat = &llvm::APFloat::IEEEsingle();
    SizeType = UnsignedInt;
    PtrDiffType = SignedInt;
    IntPtrType = SignedInt;
    Char16Type = UnsignedInt;
    WIntType = SignedInt;
    Char32Type = UnsignedLong;
    SigAtomicType = SignedChar;
    resetDataLayout(
            "e-m:e-p:16:8-i8:8-i16:8-i32:8-i64:8-a:0:8-n8-S8"
    );
  }

  void adjustTargetOptions(const CodeGenOptions &CGOpts,
                           TargetOptions &TargetOpts) const override;

  ArrayRef<const char *> getGCCRegNames() const override;

  ArrayRef<GCCRegAlias> getGCCRegAliases() const override;

  ArrayRef<Builtin::Info> getTargetBuiltins() const override;

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &info) const override {
    return false;
  }

  const char *getClobbers() const override { return ""; }

};
}
}

#endif //LLVM_CLANG_LIB_BASIC_TARGETS_TL45_H
