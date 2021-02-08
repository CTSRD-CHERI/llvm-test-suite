list(APPEND LDFLAGS -lm)
set(RUN_OPTIONS input_large.dat > /dev/null)
set(HASH_PROGRAM_OUTPUT 1)
llvm_multisource(automotive-qsort-large qsort_large.c)
llvm_test_data(automotive-qsort-large input_large.dat runme_large.sh)

set(RUN_OPTIONS input_small.dat > /dev/null)
llvm_multisource(automotive-qsort-small qsort_small.c)
llvm_test_data(automotive-qsort-small input_small.dat runme_small.sh)
