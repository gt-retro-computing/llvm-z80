//===----------------------------------------------------------------------===//
// Selection DAG types
//===----------------------------------------------------------------------===//
def SDT_CallSeqStart      : SDCallSeqStart<[SDTCisVT<0, i32>,
                                            SDTCisVT<1, i32>]>;
def SDT_CallSeqEnd        : SDCallSeqEnd<[SDTCisVT<0, i32>,
                                          SDTCisVT<1, i32>]>;

def SDT_Call        : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;


//def SDT_CondBr

//=============================================================================
// Selection DAG Nodes
//=============================================================================

def callseq_start         : SDNode<"ISD::CALLSEQ_START", SDT_CallSeqStart,
                                   [SDNPHasChain, SDNPOutGlue]>;
def callseq_end           : SDNode<"ISD::CALLSEQ_END", SDT_CallSeqEnd,
                                   [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;


def z80_call           : SDNode<"Z80ISD::CALL", SDT_Call,
                                   [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                                   SDNPVariadic]>;

def z80_ret            : SDNode<"Z80ISD::RET", SDTNone,
                                   [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;