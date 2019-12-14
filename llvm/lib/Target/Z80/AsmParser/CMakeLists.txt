add_llvm_library(LLVMZ80AsmParser
        Z80AsmParser.cpp
        )

add_dependencies(LLVMZ80AsmParser Z80CommonTableGen)