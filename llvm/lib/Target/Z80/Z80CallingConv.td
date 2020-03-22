//===- TL45CallingConv.td - Calling Conventions for TL45 -*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for TL45 architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Z80 Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_Z80 : CallingConv<[
  CCIfType<[i8], CCPromoteToType<i16>>,
  CCIfType<[i16], CCAssignToReg<[HL]>>,
  CCIfType<[i8], CCAssignToStack<4,4>>
]>;

//===----------------------------------------------------------------------===//
// Z80 Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_Z80 : CallingConv<[
  CCIfByVal<CCPassByVal<1,1>>,
  CCIfType<[i8], CCAssignToStack<1,1>>,
  CCIfVarArg<CCAssignToStack<1,1>>
]>;

def CC_Save : CalleeSavedRegs<(add)>;
