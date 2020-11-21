list(APPEND LDFLAGS -lm)
set(RUN_OPTIONS print_test_data)
llvm_multisource(security-blowfish)
llvm_test_data(security-blowfish runme_large.sh runme_small.sh)
