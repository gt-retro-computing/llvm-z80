//
// Created by codetector on 12/14/19.
//

#include "Z80TargetMachine.h"
#include "Z80.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"

using namespace llvm;


extern "C" void LLVMInitializeZ80Target() {
  RegisterTargetMachine<Z80TargetMachine> X(TheZ80Target);
}

Z80TargetMachine::Z80TargetMachine(const llvm::Target &T,
                                   const llvm::Triple &TT, llvm::StringRef CPU,
                                   llvm::StringRef FS,
                                   const llvm::TargetOptions &Options,
                                   Optional<llvm::Reloc::Model> RM,
                                   Optional<llvm::CodeModel::Model> CM,
                                   llvm::CodeGenOpt::Level OL, bool JIT) :
        LLVMTargetMachine(T,
                          "",
                          TT, CPU, FS, Options,
                          RM.getValueOr(Reloc::Model::Static),
                          CM.getValueOr(CodeModel::Tiny), OL),
        TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

TargetLoweringObjectFile *Z80TargetMachine::getObjFileLowering() const {
  return this->TLOF.get();
}

namespace {
class Z80PassConfig : public TargetPassConfig {
public:
  Z80PassConfig(Z80TargetMachine &TM, PassManagerBase &PM)
          : TargetPassConfig(TM, PM) {}

  Z80TargetMachine &getZ80TargetMachine() const {
    return getTM<Z80TargetMachine>();
  }

  bool addPreISel() override;

  bool addInstSelector() override;

  void addPreEmitPass() override;

  bool addGlobalInstructionSelect() override;
};

bool Z80PassConfig::addPreISel() {
  return false;
}

bool Z80PassConfig::addInstSelector() {
  addPass(createZ80ISelDag(getZ80TargetMachine(), getOptLevel()));
  return false;
}

void Z80PassConfig::addPreEmitPass() {
}

bool Z80PassConfig::addGlobalInstructionSelect() {
  return true; // no Global Isel
}
} // namespace

TargetPassConfig *Z80TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new Z80PassConfig(*this, PM);
//  return LLVMTargetMachine::createPassConfig(PM);
}
