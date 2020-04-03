//===----------------------------------------------------------------------===//
// Selection DAG types
//===----------------------------------------------------------------------===//
def SDT_CallSeqStart      : SDCallSeqStart<[SDTCisVT<0, i16>,
                                            SDTCisVT<1, i16>]>;
def SDT_CallSeqEnd        : SDCallSeqEnd<[SDTCisVT<0, i16>,
                                          SDTCisVT<1, i16>]>;

def SDT_Call        : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;


def SDT_SEXTLOAD_I8 : SDTypeProfile<1, 1, [SDTCisVT<0, i16>, SDTCisVT<1, iPTR>]>;
def SDT_ZEXTLOAD_I8 : SDTypeProfile<1, 1, [SDTCisVT<0, i16>, SDTCisVT<1, iPTR>]>;

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

def z80_sextload_i8 : SDNode<"Z80ISD::SEXTLOAD_I8", SDT_SEXTLOAD_I8, [SDNPHasChain]>;
def z80_zextload_i8 : SDNode<"Z80ISD::ZEXTLOAD_I8", SDT_ZEXTLOAD_I8, [SDNPHasChain]>;
