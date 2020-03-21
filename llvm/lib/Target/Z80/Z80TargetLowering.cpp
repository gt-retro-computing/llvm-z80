//
// Created by codetector on 3/21/20.
//

#include "Z80TargetLowering.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "Z80TargetMachine.h"
//#include <Z80MachineFunctionInfo.h>
#include <iostream>

#define DEBUG_TYPE "tl45-target-lowering"

using namespace llvm;

#include "Z80GenCallingConv.inc"

Z80TargetLowering::Z80TargetLowering(const Z80TargetMachine &TM,
                                     const Z80Subtarget &STI) :
        TargetLowering(TM), Subtarget(STI) {
  addRegisterClass(MVT::i8, &Z80::GR8RegClass);
  addRegisterClass(MVT::i16, &Z80::ALL16RegClass); //TODO Double check


  computeRegisterProperties(STI.getRegisterInfo());

  setStackPointerRegisterToSaveRestore(Z80::SP);
  // setOperationAction

  // END
}


SDValue Z80TargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
    default:
      llvm_unreachable("Unable to lower Operation");
  }
}

const char *Z80TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
    default:
      return nullptr;
  }
}

SDValue
Z80TargetLowering::LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                                        bool IsVarArg,
                                        const SmallVectorImpl<ISD::InputArg> &Ins,
                                        const SDLoc &DL, SelectionDAG &DAG,
                                        SmallVectorImpl<SDValue> &InVals) const {
  switch (CallConv) {
    case CallingConv::C:
    case CallingConv::Fast:
      break;
    default:
      report_fatal_error(
              "Unsupported calling conv: only FastCC and CCC is supported");
  }
  MachineFunction &MF = DAG.getMachineFunction();

  const Function &Func = MF.getFunction();
  if (Func.hasFnAttribute("interrupt")) {
    if (!Func.arg_empty())
      report_fatal_error(
              "Functions with the interrupt attribute cannot have arguments!");

    StringRef Kind =
            MF.getFunction().getFnAttribute("interrupt").getValueAsString();
  }
  return Chain;
}

SDValue Z80TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                       bool IsVarArg,
                                       const SmallVectorImpl<ISD::OutputArg> &Outs,
                                       const SmallVectorImpl<SDValue> &OutVals,
                                       const SDLoc &DL,
                                       SelectionDAG &DAG) const {
  return Chain;
}
