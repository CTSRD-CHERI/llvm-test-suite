list(APPEND LDFLAGS -lm)
set(HASH_PROGRAM_OUTPUT 1)
llvm_multisource(automotive-basicmath basicmath_large.c rad2deg.c cubic.c isqrt.c)
llvm_multisource(automotive-basicmath_small basicmath_small.c rad2deg.c cubic.c isqrt.c)
