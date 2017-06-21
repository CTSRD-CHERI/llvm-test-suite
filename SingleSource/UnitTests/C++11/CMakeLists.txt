list(APPEND CXXFLAGS -std=c++11 -pthread)
list(APPEND LDFLAGS ${TEST_SUITE_CXX_LIBRARY} -pthread)
llvm_singlesource()
