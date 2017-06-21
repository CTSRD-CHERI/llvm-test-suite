list(APPEND CXXFLAGS -pthread)
list(APPEND LDFLAGS -lm ${TEST_SUITE_CXX_LIBRARY} -pthread)
set(REQUIRES_EH_SUPPORT 1)
llvm_singlesource()
