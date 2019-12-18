include "llvm/Target/Target.td"

include "Z80RegisterInfo.td"
include "Z80InstrInfo.td"

def Z80InstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
    : Processor<Name, NoItineraries, Features>;

def : Proc<"I8080", []>;

def Z80 : Target {
    let InstructionSet = Z80InstrInfo;
}