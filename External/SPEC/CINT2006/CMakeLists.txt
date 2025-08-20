include(${CMAKE_SOURCE_DIR}/External/SPEC/SpecCPU2006.cmake)
if(TEST_SUITE_SPEC2006_ROOT)
  if (NOT CHERI_PURECAP)
    cpu2006_subdir(400.perlbench) # Broken for purecap
  endif()
  cpu2006_subdir(401.bzip2)
  if (NOT CHERI_PURECAP)
    cpu2006_subdir(403.gcc) # Broken for purecap
    cpu2006_subdir(429.mcf) # Broken for purecap
  endif()
  cpu2006_subdir(445.gobmk)
  cpu2006_subdir(456.hmmer)
  cpu2006_subdir(458.sjeng)
  cpu2006_subdir(462.libquantum)
  cpu2006_subdir(464.h264ref)
  cpu2006_subdir(471.omnetpp)
  cpu2006_subdir(473.astar)
  cpu2006_subdir(483.xalancbmk)
endif()
