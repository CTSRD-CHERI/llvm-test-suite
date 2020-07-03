list(APPEND CPPFLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/include -std=gnu++98 -DNO_TIME)
list(APPEND LDFLAGS ${TEST_SUITE_CXX_LIBRARY} -lm)
llvm_multisource(bullet)
