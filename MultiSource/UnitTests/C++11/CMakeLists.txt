list(APPEND CXXFLAGS -std=c++11)

if(NOT MIPS_IS_MIPS16_ENABLED)
  # RISCV: fatal error: error in backend: Incomplete scavenging after 2nd pass
  if (NOT ARCH MATCHES "riscv")
    add_subdirectory(frame_layout)
  endif()
endif()
