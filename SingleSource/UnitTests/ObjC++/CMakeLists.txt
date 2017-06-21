list(APPEND LDFLAGS ${TEST_SUITE_CXX_LIBRARY} -lobjc -framework Foundation)
file(GLOB Source *.mm)
llvm_singlesource()
