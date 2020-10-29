list(APPEND LDFLAGS -lm)
list(APPEND CXXFLAGS -Wno-error=c++11-narrowing)
llvm_multisource(assembler)
