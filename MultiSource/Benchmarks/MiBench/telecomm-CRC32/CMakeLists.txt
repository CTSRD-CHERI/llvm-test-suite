list(APPEND LDFLAGS -lm)
set(RUN_OPTIONS large.pcm)
llvm_multisource(telecomm-crc32)
llvm_test_data(telecomm-crc32 large.pcm small.pcm runme_large.sh runme_small.sh)
