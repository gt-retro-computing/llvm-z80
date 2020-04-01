class Z80Reg<bits<16> Enc, string n> : Register<n> {
    let HWEncoding = Enc;
    let Namespace = "Z80";
    int SpillSize = 8;
    int SpillAlignment = 8;
}

def LEFT_REG : SubRegIndex<8, 0>;
def RIGHT_REG : SubRegIndex<8, 8>;

class Z80RegPair<bits<16> Enc, string n, list<Z80Reg> parts> : Register<n> {
    let HWEncoding = Enc;
    let Namespace = "Z80";
    int SpillSize = 16;
    int SpillAlignment = 8;
    let SubRegs = parts;
    let SubRegIndices = [LEFT_REG, RIGHT_REG];
}

class Z80BigReg<bits<16> Enc, string n> : Register<n> {
    let HWEncoding = Enc;
    let Namespace = "Z80";
    int SpillSize = 16;
    int SpillAlignment = 8;
}

def B : Z80Reg<0, "b">;
def C : Z80Reg<1, "c">;
def D : Z80Reg<2, "d">;
def E : Z80Reg<3, "e">;
def H : Z80Reg<4, "h">;
def L : Z80Reg<5, "l">;
def flags : Z80Reg<6, "f">;


def BC : Z80RegPair<0, "bc", [C,B]>;
def DE : Z80RegPair<1, "de", [E,D]>;
def HL : Z80RegPair<2, "hl", [L,H]>;
def SP : Z80BigReg <3, "sp">;

def ACC : Z80Reg<7, "a">;

def GR8 : RegisterClass<"Z80", [i8], 8,
(add
B,C,D,E,H,L,ACC
)>;

def AccReg : RegisterClass<"Z80", [i8], 8,
(add ACC)>;

def HLReg : RegisterClass<"Z80", [i16], 8, (add HL)>;

def BCDEReg : RegisterClass<"Z80", [i16], 8, (add BC, DE)>;

def SPReg: RegisterClass<"Z80", [i16], 8, (add SP)>;

def ALL16 : RegisterClass<"Z80", [i16], 8,
(add
BC, DE, HL, SP
)
>;

def GR16 : RegisterClass<"Z80", [i16], 8,
(add
BC, DE, HL
)
>;