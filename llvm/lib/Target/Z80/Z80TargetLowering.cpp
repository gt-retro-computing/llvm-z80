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
    case Z80ISD::RET:
      return "Z80ISD::RET";
    case Z80ISD::CALL:
      return "Z80ISD::CALL";
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

static SDValue convertValVTToLocVT(SelectionDAG &DAG, SDValue Val,
                                   const CCValAssign &VA, const SDLoc &DL) {
  EVT LocVT = VA.getLocVT();

  switch (VA.getLocInfo()) {
    default:
      llvm_unreachable("Unexpected CCValAssign::LocInfo");
    case CCValAssign::Full:
    case CCValAssign::AExt:
      break;
    case CCValAssign::BCvt:
      Val = DAG.getNode(ISD::BITCAST, DL, LocVT, Val);
      break;
  }
  return Val;
}

SDValue Z80TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                       bool IsVarArg,
                                       const SmallVectorImpl<ISD::OutputArg> &Outs,
                                       const SmallVectorImpl<SDValue> &OutVals,
                                       const SDLoc &DL,
                                       SelectionDAG &DAG) const {
  switch (CallConv) {
    case CallingConv::Fast:
    case CallingConv::C:
      break;
    default:
      report_fatal_error("Only FastCC and CCC is supported");
  }

  // Stores the assignment of the return value to a location.
  SmallVector<CCValAssign, 16> RVLocs;

  // Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  analyzeOutputArgs(DAG.getMachineFunction(), CCInfo, Outs, /*IsRet=*/true,
                    nullptr);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];

    Val = convertValVTToLocVT(DAG, Val, VA, DL);
    if (VA.isRegLoc()) {
      // Return value should store to a register
      Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Glue);

      // Guarantee that all emitted copies are stuck together.
      Glue = Chain.getValue(1);
      RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
    } else if (VA.isMemLoc()) {
      report_fatal_error("Stack Return is not supported");
    } else {
      LLVM_DEBUG(dbgs() << "Trying to return VAType: "
                        << EVT(VA.getLocVT()).getEVTString()
                        << "\n");
      llvm_unreachable("Unsupported Return Value / Location");
    }
  }

  RetOps[0] = Chain; // Update chain.

  // Add the glue node if we have it.
  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(Z80ISD::RET, DL, MVT::Other, RetOps);
}

void Z80TargetLowering::analyzeOutputArgs(
        MachineFunction &MF, CCState &CCInfo,
        const SmallVectorImpl<ISD::OutputArg> &Outs, bool IsRet,
        CallLoweringInfo *CLI) const {
  unsigned NumArgs = Outs.size();

  for (unsigned i = 0; i != NumArgs; i++) {
    MVT ArgVT = Outs[i].VT;
    ISD::ArgFlagsTy ArgFlags = Outs[i].Flags;
    // Type *OrigTy = CLI ? CLI->getArgs()[Outs[i].OrigArgIndex].Ty : nullptr;

    if (IsRet) {
      // callee emitting return values
      if (RetCC_Z80(i, ArgVT, ArgVT, CCValAssign::Full, ArgFlags, CCInfo)) {
        LLVM_DEBUG(dbgs() << "OutputArg #" << i << "/" << NumArgs
                          << " has unhandled type "
                          << EVT(ArgVT).getEVTString() << "\n");
        llvm_unreachable(nullptr);
      }
    } else {
      // caller emitting args
      if (CC_Z80(i, ArgVT, ArgVT, CCValAssign::Full, ArgFlags, CCInfo)) {
        LLVM_DEBUG(dbgs() << "CC_Z80: OutputArg #" << i << "/" << NumArgs
                          << " has unhandled type "
                          << EVT(ArgVT).getEVTString() << "\n");
        llvm_unreachable(nullptr);
      }
    }
  }
}
