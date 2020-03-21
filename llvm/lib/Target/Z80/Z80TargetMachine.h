//
// Created by codetector on 12/14/19.
//

#ifndef LLVM_Z80TARGETMACHINE_H
#define LLVM_Z80TARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"
#include "Z80Subtarget.h"

namespace llvm {
class Z80TargetMachine : public LLVMTargetMachine {
private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  Z80Subtarget DefaultSubtarget;
public:
  Z80TargetMachine(const llvm::Target &T, const llvm::Triple &TT, llvm::StringRef CPU,
                    llvm::StringRef FS, const llvm::TargetOptions &Options,
                    Optional<llvm::Reloc::Model> RM,
                    Optional<llvm::CodeModel::Model> CM, llvm::CodeGenOpt::Level OL, bool JIT);

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override;
};
}

#endif //LLVM_Z80TARGETMACHINE_H
