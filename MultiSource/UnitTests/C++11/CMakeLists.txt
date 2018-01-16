list(APPEND CXXFLAGS -std=c++11)

# FIXME: crashes with CHERI
if(NOT MIPS_IS_MIPS16_ENABLED AND NOT CHERI)
  add_subdirectory(frame_layout)
endif()
