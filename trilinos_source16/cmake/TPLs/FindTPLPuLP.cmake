
IF (TPL_ENABLE_MPI)
   TRIBITS_TPL_FIND_INCLUDE_DIRS_AND_LIBRARIES( PuLP
    REQUIRED_HEADERS xtrapulp.h
    REQUIRED_LIBS_NAMES xtrapulp
   )
ELSE()
   TRIBITS_TPL_FIND_INCLUDE_DIRS_AND_LIBRARIES( PuLP
    REQUIRED_HEADERS pulp.h
    REQUIRED_LIBS_NAMES pulp
   )
ENDIF()