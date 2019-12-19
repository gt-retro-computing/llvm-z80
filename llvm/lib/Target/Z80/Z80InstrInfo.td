include "Z80InstrFormats.td"

// ================
// 8 Bit Load Group
// ================

def Z80LD8rr : InstZ80<(outs GR8:$dr), (ins GR8:$sr),
 "ld", "$dr, $sr",
 [(set i8:$dr, i8:$sr)]
>;

def Z80LD8ri : InstZ80<(outs GR8:$dr), (ins imm8:$imm),
 "ld", "$dr, ${imm}",
 [(set i8:$dr, i8:$imm)]
>;

def Z80LDrHL : InstZ80<(outs GR8:$dr), (ins HLReg:$hlref),
 "ld", "$dr, (${hlref})",
 [(set i8:$dr, (load i16:$hlref))]
>;
