#!/bin/bash
rm -rf CMakeCache.txt CMakeFiles
cmake \
-D CMAKE_INSTALL_PREFIX="/home/rppawlo/JUNK" \
-D Trilinos_ENABLE_DEBUG=OFF \
-D Trilinos_ENABLE_EXAMPLES:BOOL=OFF \
-D Trilinos_ENABLE_TESTS:BOOL=OFF \
-D Trilinos_ENABLE_ALL_PACKAGES:BOOL=OFF \
-D Trilinos_ENABLE_ALL_OPTIONAL_PACKAGES:BOOL=OFF \
-D Trilinos_ENABLE_KokkosCore:BOOL=ON \
-D Trilinos_ENABLE_KokkosAlgorithms:BOOL=ON \
-D Kokkos_ENABLE_Cuda=ON \
-D Kokkos_ENABLE_Cuda_UVM=ON \
-D Trilinos_ENABLE_Intrepid2:BOOL=ON \
-D Trilinos_ENABLE_Phalanx:BOOL=ON \
-D Phalanx_KOKKOS_DEVICE_TYPE:STRING="CUDA" \
-D Phalanx_INDEX_SIZE_TYPE:STRING="INT" \
-D Phalanx_ENABLE_DEBUG:BOOL=OFF \
-D Phalanx_ENABLE_TESTS:BOOL=ON \
-D Phalanx_ENABLE_EXAMPLES:BOOL=ON \
-D Phalanx_EXPLICIT_TEMPLATE_INSTANTIATION=ON \
-D Phalanx_ALLOW_MULTIPLE_EVALUATORS_FOR_SAME_FIELD:BOOL=OFF \
-D HAVE_INTREPID_KOKKOS:BOOL=ON \
-D TPL_ENABLE_CUDA=ON \
-D TPL_ENABLE_MPI:BOOL=ON \
-D MPI_BASE_DIR:PATH="$SEMS_OPENMPI_ROOT" \
-D TPL_ENABLE_HWLOC:BOOL=OFF \
-D HWLOC_INCLUDE_DIRS:FILEPATH="/home/rppawlo/install/gnu4.8.2/hwloc/include" \
-D HWLOC_LIBRARY_DIRS:FILEPATH="/home/rppawlo/install/gnu4.8.2/hwloc/lib" \
-D TPL_BLAS_LIBRARIES:PATH="$ROGER_BLAS_LIBRARY" \
-D TPL_LAPACK_LIBRARIES:PATH="$ROGER_LAPACK_LIBRARY" \
-D CMAKE_CXX_COMPILER:FILEPATH="$SEMS_OPENMPI_ROOT/bin/mpicxx" \
-D CMAKE_C_COMPILER:FILEPATH="$SEMS_OPENMPI_ROOT/bin/mpicc" \
-D CMAKE_Fortran_COMPILER:FILEPATH="$SEMS_OPENMPI_ROOT/bin/mpifort" \
-D CMAKE_CXX_FLAGS:STRING="-g -lineinfo -Xcudafe \
--diag_suppress=conversion_function_not_usable -Xcudafe \
--diag_suppress=cc_clobber_ignored -Xcudafe \
--diag_suppress=code_is_unreachable" \
-D CMAKE_VERBOSE_MAKEFILE:BOOL=OFF \
-D Trilinos_VERBOSE_CONFIGURE:BOOL=OFF \
-D CMAKE_SKIP_RULE_DEPENDENCY=ON \
-D CMAKE_BUILD_TYPE:STRING=RELEASE \
-D Trilinos_ENABLE_INSTALL_CMAKE_CONFIG_FILES:BOOL=OFF \
-D Trilinos_ENABLE_EXPORT_MAKEFILES:BOOL=OFF \
-D Trilinos_DEPS_XML_OUTPUT_FILE:FILEPATH="" \
../Trilinos