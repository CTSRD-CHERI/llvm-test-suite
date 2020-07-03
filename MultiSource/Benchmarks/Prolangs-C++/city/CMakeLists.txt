list(APPEND LDFLAGS ${TEST_SUITE_CXX_LIBRARY})
llvm_multisource(city)
target_include_directories(city PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
