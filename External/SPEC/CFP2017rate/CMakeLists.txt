if (TEST_SUITE_FORTRAN)
  add_subdirectory(503.bwaves_r)    # Fortran
  add_subdirectory(507.cactuBSSN_r) # C++, C, Fortran
endif ()
add_subdirectory(508.namd_r)       # C++
add_subdirectory(510.parest_r)     # C++
add_subdirectory(511.povray_r)     # C++, C
add_subdirectory(519.lbm_r)        # C
if (TEST_SUITE_FORTRAN)
  add_subdirectory(521.wrf_r)      # Fortran, C
endif ()
if (NOT CHERI_PURECAP)
  # The config files serialize pointers and other code triggers a
  # -Werror,-Wpadded that cannot be disabled without patching the sources.
  # Skip for purecap since it requires non-trivial patches.
  add_subdirectory(526.blender_r)    # C++, C
endif()
if (TEST_SUITE_FORTRAN)
  add_subdirectory(527.cam4_r)     # Fortran, C
endif ()
add_subdirectory(538.imagick_r)    # C
add_subdirectory(544.nab_r)        # C
if (TEST_SUITE_FORTRAN)
  add_subdirectory(549.fotonik3d_r) # Fortran
  add_subdirectory(554.roms_r)      # Fortran
endif ()

add_subdirectory(997.specrand_fr)  # C
