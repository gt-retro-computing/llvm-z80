include "Z80InstrFormats.td"

include "Z80ISDNodes.td"


// ===========
// 16 bit Arithmetic Group
// ===========

def ADD16HLr: InstZ80<(outs HLReg:$hl), (ins HLReg:$hl_src, ALL16:$src),
"add", "$hl, $src",
[(set i16:$hl, (add i16:$hl_src, i16:$src))]>;

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
 [(set (i8 GR8:$dr), (load HLReg:$hlref))]
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

// =======================
// 8 Bit Arithmetic Group
// =======================
let Defs = [flags] in {
let isCommutable = 0, isAdd = 1 in {
    defm ADD : Z80ALUInst<"add", add>;
} // isAdd = 1
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

// Br $target -> JMP
def : Pat<(br bb:$target), (JP branch_target:$target)>;

let isCall=1 in {
    let Defs = [SP] in
    def CALL : InstZ80<(outs), (ins imm16:$target),
    "call", "$target", []>;
}

let isReturn=1, isBarrier=1, isTerminator=1 in {
    let Defs = [SP] in
        def RET: InstZ80<(outs), (ins), "ret", "", [(z80_ret)]>;
}


// =============
// 16 Bits Load Group
// =============

def LD16ri : InstZ80<(outs ALL16:$dst), (ins imm16:$imm),
    "ld", "$dst, $imm",
    [(set i16:$dst, (i16 imm16:$imm))]>;

def LDSPHL : InstZ80<(outs SPReg:$sp), (ins HLReg:$hl),
"ld", "$sp, $hl",
[]>;

def LDHLIndi : InstZ80<(outs HLReg:$hl), (ins imm16:$addr),
"ld", "$hl, (${addr})",
[(set (i16 HLReg:$hl), (load imm16:$addr))]>;


// ========
// PUSH POP
// ========

def PUSHRP : InstZ80<(outs), (ins GR16:$rp),
"push", "$rp",
[]>;

def POPRP: InstZ80<(outs GR16:$rp), (ins),
"pop", "$rp",
[]>;


// ===========
// PSEUDO Group
// ===============
let Defs=[ACC, flags] in
def SEXTLOAD_I8 : PseudoZ80<(outs GR16:$dst), (ins imm16:$addr),
[(set GR16:$dst, (z80_sextload_i8 imm16:$addr))]>;