include(CheckFunctionExists)
check_function_exists(re_comp HAVE_RE_COMP)

if(HAVE_RE_COMP)
  set(PROG archie)
  if(TARGET_OS STREQUAL "SunOS")
    list(APPEND LDFLAGS -lsocket -lnsl)
  elseif(TARGET_OS MATCHES "FreeBSD")
    # re_comp is in libcompat on FreeBSD
    list(APPEND LDFLAGS -lcompat)
  endif()
  llvm_multisource()
endif()
