include "Z80InstrFormats.td"

include "Z80ISDNodes.td"

// ================
// 8 Bit Load Group
// ================

def LD8rr : InstZ80<(outs GR8:$dr), (ins GR8:$sr),
 "ld", "$dr, $sr",
 [(set i8:$dr, i8:$sr)]
>;

def LD8ri : InstZ80<(outs GR8:$dr), (ins imm8:$imm),
 "ld", "$dr, ${imm}",
 [(set i8:$dr, i8:$imm)]
>;

def LDrHL : InstZ80<(outs GR8:$dr), (ins HLReg:$hlref),
 "ld", "$dr, (${hlref})",
 [(set i8:$dr, (load i16:$hlref))]
>;

def LDHLr : InstZ80<(outs), (ins HLReg:$hlref, GR8:$sr),
 "ld", "(${hlref}), $sr",
 [(store i8:$sr, i16:$hlref)]
>;

// LD (HL), imm
def LDHLi : InstZ80<(outs), (ins HLReg:$hlref, imm8:$imm),
 "ld", "(${hlref}), $imm",
 [(store i8:$imm, i16:$hlref)]
>;

// LD A, (BC/DE)
def LDAccBCDE : InstZ80<(outs AccReg:$a), (ins BCDEReg:$bcde),
 "ld", "$a, (${bcde})",
 [(set i8:$a, (load i16:$bcde))]
>;

// LD (imm), A
let mayStore = 1 in
def LD_StoreAi : InstZ80<(outs), (ins AccReg:$a, imm16:$imm),
 "ld", "(${imm}), $a",
 [(store i8:$a, i16:$imm)]
>;

def LD16ri : InstZ80<(outs ALL16:$dst), (ins imm16:$imm),
    "ld", "$dst, $imm",
    [(set i16:$dst, i16:$imm)]>;

// =======================
// 8 Bit Arithmetic Group
// =======================
let Defs = [flags] in {
let isCommutable = 1, isAdd = 1 in {
    defm ADD : Z80ALUInst<"add", add>;
} // let isCommutable = 1 & isAdd = 1
let Uses = [flags] in {
    defm ADC : Z80ALUInst<"adc", addc>;
    defm SUBC : Z80ALUInst<"subc", subc>;
} // let Uses = [flags]
defm SUB : Z80ALUInst<"sub", sub>;
defm AND : Z80ALUInst<"and", and>;
defm OR : Z80ALUInst<"or", or>;
defm XOR : Z80ALUInst<"xor", xor>;
} // let def=[flags]


let isBranch = 1 in {
let isTerminator=1, isBarrier=1 in
    def JP : InstZ80<(outs), (ins branch_target:$addr),
    "jp", "$addr",
    []
    >;
    let Uses = [flags], isTerminator = 1, isPredicable = 1 in
        def JPcc : InstZ80<(outs), (ins branch_cc: $cc, branch_target:$addr),
        "jp", "$cc, $addr",
        []
        >;
}

let isCall=1 in {
    let Defs = [SP] in
    def CALL : InstZ80<(outs), (ins imm16:$target),
    "call", "$target", []>;
}

let isReturn=1, isBarrier=1, isTerminator=1 in {
    let Defs = [SP] in
        def RET: InstZ80<(outs), (ins), "ret", "", [(z80_ret)]>;
}

