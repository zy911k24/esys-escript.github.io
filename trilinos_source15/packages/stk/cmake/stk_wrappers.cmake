
macro(STK_ADD_DEBUG_AND_DEPRECATED_OPTIONS)
  if(HAVE_STK_Trilinos)
    TRIBITS_ADD_DEBUG_OPTION()
    TRIBITS_ADD_SHOW_DEPRECATED_WARNINGS_OPTION()
  else()
    # what should we do here?
  endif()
endmacro()

macro(STK_CONFIGURE_FILE filename)
  if(HAVE_STK_Trilinos)
    TRIBITS_CONFIGURE_FILE(${filename})
    TRIBITS_INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  else()
    configure_file(${STK_TOPLEVEL_SOURCE_DIR}/cmake/${filename}.in ${CMAKE_CURRENT_BINARY_DIR}/${filename})
  endif()
endmacro()

function(stk_process_enables)
  if(STK_ENABLE_ALL)
    set(STK_ENABLE_STKUtil ON PARENT_SCOPE)

    if (DEFINED STK_ENABLE_STKCoupling AND NOT STK_ENABLE_STK_Coupling)
      message("STKCoupling explicitly disabled.")
    elseif(NOT STK_HAS_MPI)
      message("STKCoupling disabled because MPI not enabled.")
    else()
      set(STK_ENABLE_STKCoupling ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKMath AND NOT STK_ENABLE_STK_Math)
      message("STKMath explicitly disabled.")
    else()
      set(STK_ENABLE_STKMath ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKSimd AND NOT STK_ENABLE_STK_Simd)
      message("STKSimd explicitly disabled.")
    else()
      set(STK_ENABLE_STKSimd ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKNGP_TEST AND NOT STK_ENABLE_STK_NGP_TEST)
      message("STKNGP_TEST explicitly disabled.")
    else()
      set(STK_ENABLE_STKNGP_TEST ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKExprEval AND NOT STK_ENABLE_STKExprEval)
      message("STKExprEval explicitly disabled.")
    else()
      set(STK_ENABLE_STKExprEval ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKTopology AND NOT STK_ENABLE_STK_Topology)
      message("STKTopology explicitly disabled.")
    else()
      set(STK_ENABLE_STKTopology ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKSearch AND NOT STK_ENABLE_STK_Search)
      message("STKSearch explicitly disabled.")
    elseif(NOT STK_HAS_MPI)
      message("STKSearch disabled because MPI not enabled.")
    else()
      set(STK_ENABLE_STKSearch ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKMiddle_mesh AND NOT STK_ENABLE_STK_Middle_mesh)
      message("STKMiddle_mesh explicitly disabled.")
    else()
      set(STK_ENABLE_STKMiddle_mesh ON PARENT_SCOPE)
    endif()

    if (DEFINED STK_ENABLE_STKTransfer AND NOT STK_ENABLE_STK_Transfer)
      message("STKTransfer explicitly disabled.")
    elseif(NOT STK_ENABLE_STKSearch)
      message("STKTransfer disabled because STKSearch not enabled.")
    else()
      set(STK_ENABLE_STKTransfer ON PARENT_SCOPE)
    endif()
  endif()
  
  if(STK_ENABLE_TESTS)
    set(STK_ENABLE_STKUnit_tests ON PARENT_SCOPE)
    set(STK_ENABLE_STKDoc_tests ON PARENT_SCOPE)
  endif()
endfunction()

macro(STK_SUBPACKAGES)
  if(HAVE_STK_Trilinos)
    TRIBITS_PROCESS_SUBPACKAGES()
  else()
    add_subdirectory(stk_util)
    message("STKUtil is always enabled.")

    if(STK_ENABLE_STKCoupling)
      add_subdirectory(stk_coupling)
    endif()

    if(STK_ENABLE_STKMath)
      add_subdirectory(stk_math)
      message("STKMath is enabled.")
    endif()

    if(STK_ENABLE_STKSimd)
      add_subdirectory(stk_simd)
      message("STKSimd is enabled.")
    endif()

    if(STK_ENABLE_STKNGP_TEST)
      add_subdirectory(stk_ngp_test)
      message("STKNGP_TEST is enabled.")
    endif()

    if(STK_ENABLE_STKExprEval)
      add_subdirectory(stk_expreval)
      message("STKExprEval is enabled.")
    endif()

    if(STK_ENABLE_STKTopology)
      add_subdirectory(stk_topology)
      message("STKTopology is enabled.")
    endif()

    if(STK_ENABLE_STKSearch)
      add_subdirectory(stk_search)
      message("STKSearch is enabled.")
    endif()

    if(STK_ENABLE_STKMiddle_mesh)
      add_subdirectory(stk_middle_mesh)
      message("STKMiddle_mesh is enabled.")
    endif()

    if(STK_ENABLE_STKTransfer)
      add_subdirectory(stk_transfer)
      message("STKTransfer is enabled.")
    endif()

    if(STK_ENABLE_TESTS)
      add_subdirectory(stk_unit_test_utils)
      message("STKUnit_test_utils is enabled.")

      add_subdirectory(stk_unit_tests)
      message("STKUnit_tests is enabled.")
    endif()
  endif()
endmacro()

macro(STK_SUBPACKAGE subpkg)
  if(HAVE_STK_Trilinos)
    TRIBITS_SUBPACKAGE($subpkg)
  else()
  endif()
endmacro()

macro(STK_ADD_TEST_DIRECTORIES subdir)
  if(HAVE_STK_Trilinos)
    TRIBITS_ADD_TEST_DIRECTORIES(${subdir})
  else()
    add_subdirectory(${subdir})
  endif()
endmacro()

macro(STK_SUBPACKAGE_POSTPROCESS)
  if(HAVE_STK_Trilinos)
    TRIBITS_SUBPACKAGE_POSTPROCESS()
  else()
  endif()
endmacro()

macro(STK_PACKAGE_POSTPROCESS)
  if(HAVE_STK_Trilinos)
    TRIBITS_PACKAGE_DEF()
    TRIBITS_PACKAGE_POSTPROCESS()
  else()
  endif()
endmacro()