list(APPEND CXXFLAGS -std=gnu++98)
list(APPEND LDFLAGS -lm ${TEST_SUITE_CXX_LIBRARY})
set(FP_TOLERANCE 0.001)
llvm_singlesource()
