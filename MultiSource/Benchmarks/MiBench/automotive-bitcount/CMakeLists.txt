list(APPEND LDFLAGS -lm)
set(RUN_OPTIONS 1125000)
llvm_multisource(automotive-bitcount)
llvm_test_data(automotive-bitcount runme_large.sh runme_small.sh)
