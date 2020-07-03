list(APPEND CXXFLAGS -pthread)
list(APPEND LDFLAGS -lm ${TEST_SUITE_CXX_LIBRARY} -pthread)
llvm_singlesource()
