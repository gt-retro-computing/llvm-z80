//
// Created by codetector on 3/21/20.
//

#ifndef LLVM_Z80ISELLOWERING_H
#define LLVM_Z80ISELLOWERING_H

#include "Z80.h"
#include "MCTargetDesc/Z80MCTargetDesc.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/MachineValueType.h"
#include "llvm/Target/TargetMachine.h"
#include <algorithm>
#include <cassert>
#include <deque>
#include <string>
#include <utility>
#include <vector>

namespace llvm {

class Argument;

class CCState;

class CCValAssign;

class FastISel;

class FunctionLoweringInfo;

class MachineBasicBlock;

class MachineFrameInfo;

class MachineInstr;

class Z80CCState;

class Z80FunctionInfo;

class Z80Subtarget;

class Z80TargetMachine;

class TargetLibraryInfo;

class TargetRegisterClass;

namespace Z80ISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  CALL,
  RET,
  SEXTLOAD_I8,
  ZEXTLOAD_I8,
};
} // namespace Z80ISD

//===--------------------------------------------------------------------===//
// TargetLowering Implementation
//===--------------------------------------------------------------------===//

class Z80TargetLowering : public TargetLowering {
  const Z80Subtarget &Subtarget;

public:
  explicit Z80TargetLowering(const Z80TargetMachine &TM,
                              const Z80Subtarget &STI);

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

//  SDValue LowerCall(CallLoweringInfo & /*CLI*/,
//                    SmallVectorImpl<SDValue> & /*InVals*/) const override;

  // Provide custom lowering hooks for some operations.
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  const char *getTargetNodeName(unsigned Opcode) const override;

//  std::pair<unsigned, const TargetRegisterClass *>
//  getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
//                               StringRef Constraint,
//                               MVT VT) const override;

//  MachineBasicBlock * EmitInstrWithCustomInserter(MachineInstr &MI,
//                                                  MachineBasicBlock *BB) const override;
//
//  bool shouldReduceLoadWidth(SDNode *Load, ISD::LoadExtType ExtTy, EVT NewVT) const override;
//
//  bool canMergeStoresTo(unsigned AS, EVT MemVT, const SelectionDAG &DAG) const override;



private:
//  void analyzeInputArgs(MachineFunction &MF, CCState &CCInfo,
//                        const SmallVectorImpl<ISD::InputArg> &Ins,
//                        bool IsRet) const;
//
  void analyzeOutputArgs(MachineFunction &MF, CCState &CCInfo,
                         const SmallVectorImpl<ISD::OutputArg> &Outs,
                         bool IsRet, CallLoweringInfo *CLI) const;
//
//  // copied from LegalizeDAG. We emit our own libcalls not directly supported by LLVM.
//  SDValue ExpandLibCall(const char *LibcallName, SDValue Op, bool isSigned, SelectionDAG &DAG) const;
//
  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerLoad(SDValue Op, SelectionDAG &DAG) const ;
//  SDValue lowerBr(SDValue Op, SelectionDAG &DAG) const;
//
//  SDValue lowerSelectCc(SDValue Op, SelectionDAG &DAG) const;
//
//  SDValue lowerSELECT(SDValue Op, SelectionDAG &DAG) const;
//
//  SDValue lowerVASTART(SDValue Op, SelectionDAG &DAG) const;
//
//  template <class NodeTy>
//  SDValue getAddr(NodeTy *N, SelectionDAG &DAG, bool IsLocal) const;
//
//  SDValue lowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
};

} // end namespace llvm
#endif //LLVM_Z80ISELLOWERING_H
