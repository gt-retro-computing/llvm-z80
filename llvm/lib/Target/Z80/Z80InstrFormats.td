

// Operand Types
def imm8: Operand<i8>, ImmLeaf<i8, [{return isInt<8>(Imm);}]> {
  let EncoderMethod = "getImmOpValue";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<8>(Imm);
    return MCOp.isBareSymbolRef();
  }];
}


class InstZ80<dag outs, dag ins,
string opcodestr, string argstr, list<dag> pattern> : Instruction
{
  field bits<32> Inst;

  let Namespace = "Z80";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = opcodestr # "\t" # argstr;
  let Pattern = pattern;
  let Size = 4; //???

  // SoftFail is a field the disassembler can use to provide a way for
  // instructions to not match without killing the whole decode process. It is
  // mainly used for ARM, but Tablegen expects this field to exist or it fails
  // to build the decode table.
  field bits<32> SoftFail = 0;
}
