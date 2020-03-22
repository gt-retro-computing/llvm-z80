//
// Created by codetector on 12/16/19.
//

#include "Z80InstrInfo.h"
#include "MCTargetDesc/Z80MCTargetDesc.h"

#define DEBUG_TYPE "Z80-InstrInfo"

#define GET_INSTRINFO_CTOR_DTOR

#include "Z80GenInstrInfo.inc"

using namespace llvm;

Z80InstrInfo::Z80InstrInfo() : Z80GenInstrInfo(-1, -1, -1, -1), RI() {}

static unsigned getRegSize(MCRegister Reg) {
  switch (Reg) {
    case Z80::BC:
    case Z80::DE:
    case Z80::HL:
    case Z80::SP:
      return 16;
    default:
      return 8;
  }
}

static MCRegister getPairHigh(MCRegister Reg) {
  switch (Reg) {
    case Z80::BC:
      return Z80::B;
    case Z80::DE:
      return Z80::D;
    case Z80::HL:
      return Z80::H;
    default:
      llvm_unreachable("Specified reg is not a pair");
  }
}


static MCRegister getPairLow(MCRegister Reg) {
  switch (Reg) {
    case Z80::BC:
      return Z80::C;
    case Z80::DE:
      return Z80::E;
    case Z80::HL:
      return Z80::L;
    default:
      llvm_unreachable("Specified reg is not a pair");
  }
}

void Z80InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator MI,
                               const DebugLoc &DL, MCRegister DestReg,
                               MCRegister SrcReg, bool KillSrc) const {
  unsigned SrcSize, DstSize;
  SrcSize = getRegSize(SrcReg);
  DstSize = getRegSize(DestReg);
  assert(DstSize >= SrcSize && "Destination is smaller than source can't copy");
  if (DstSize == 8) {
    BuildMI(MBB, MI, DL, get(Z80::LD8rr), DestReg).addReg(SrcReg);
  } else {
    if (SrcSize == 16) {
      BuildMI(MBB, MI, DL, get(Z80::LD8rr), getPairHigh(DestReg)).addReg(
              getPairHigh(SrcReg));
      BuildMI(MBB, MI, DL, get(Z80::LD8rr), getPairLow(DestReg)).addReg(
              getPairLow(SrcReg));
    } else {
      BuildMI(MBB, MI, DL, get(Z80::LD8ri), getPairHigh(DestReg)).addImm(0);
      BuildMI(MBB, MI, DL, get(Z80::LD8rr), getPairLow(DestReg)).addReg(SrcReg);
    }
  }
}
