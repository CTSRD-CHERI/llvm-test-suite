list(APPEND CXXFLAGS -std=c++11)

if(NOT MIPS_IS_MIPS16_ENABLED)
  # RISCV: fatal error: error in backend: Incomplete scavenging after 2nd pass
  # Also a problem for CHERI: https://github.com/CTSRD-CHERI/llvm-project/issues/484
  if (NOT ARCH MATCHES "riscv" AND NOT ARCH MATCHES "mips")
    add_subdirectory(frame_layout)
  endif()
endif()
