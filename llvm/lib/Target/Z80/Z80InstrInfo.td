include "Z80InstrFormats.td"

def Z80LD8rr : InstZ80<(outs GR8:$dr), (ins GR8:$sr),
 "LD", "$dr, $sr",
 [(set i8:$dr, i8:$sr)]
 >;