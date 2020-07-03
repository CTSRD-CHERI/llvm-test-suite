add_subdirectory(Large)

list(APPEND LDFLAGS -lm ${TEST_SUITE_CXX_LIBRARY})
set(FP_ABSTOLERANCE 0.01)
llvm_singlesource()
