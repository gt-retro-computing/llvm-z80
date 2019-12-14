
set(LLVM_TARGET_DEFINITIONS Z80.td)

# Table Gens
tablegen(LLVM Z80GenAsmMatcher.inc -gen-asm-matcher)

# LLVM Targets

# Subdirectories
add_subdirectory(AsmParser)