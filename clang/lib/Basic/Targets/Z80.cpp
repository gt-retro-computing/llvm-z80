//
// Created by codetector on 4/1/20.
//

#include "Z80.h"
#include <clang/Basic/TargetBuiltins.h>

using namespace clang;
using namespace clang::targets;

const char *const Z80TargetInfo::GCCRegNames[] = {
        "a", "b", "c", "d",
        "e", "h", "l", "bc",
        "de", "hl", "sp",
        NULL
};

ArrayRef<const char *> Z80TargetInfo::getGCCRegNames() const {
  return llvm::makeArrayRef(GCCRegNames);
}

const TargetInfo::GCCRegAlias Z80TargetInfo::GCCRegAliases[] = {
        {{}, "a"},
        {{}, "b"},
        {{}, "c"},
        {{}, "d"},
        {{}, "e"},
        {{}, "h"},
        {{}, "l"},
        {{}, "bc"},
        {{}, "de"},
        {{}, "hl"},
        {{}, "sp"},
};

ArrayRef<TargetInfo::GCCRegAlias> Z80TargetInfo::getGCCRegAliases() const {
  return llvm::makeArrayRef(GCCRegAliases);
}

ArrayRef<Builtin::Info> Z80TargetInfo::getTargetBuiltins() const {
  return ArrayRef<Builtin::Info>();
//  return llvm::makeArrayRef(BuiltinInfo, clang::TL45::LastTSBuiltin -
//                                         Builtin::FirstTSBuiltin);
}



void Z80TargetInfo::getTargetDefines(const clang::LangOptions &Opts, clang::MacroBuilder &Builder) const {
}

void Z80TargetInfo::adjustTargetOptions(const CodeGenOptions &CGOpts,
                                        TargetOptions &TargetOpts) const {
  TargetInfo::adjustTargetOptions(CGOpts, TargetOpts);
}

