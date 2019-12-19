//
// Created by codetector on 12/16/19.
//

#include "Z80InstrInfo.h"

#define DEBUG_TYPE "Z80-InstrInfo"

#define GET_INSTRINFO_CTOR_DTOR

#include "Z80GenInstrInfo.inc"

using namespace llvm;

Z80InstrInfo::Z80InstrInfo() : Z80GenInstrInfo(-1, -1, -1, -1), RI() {}
