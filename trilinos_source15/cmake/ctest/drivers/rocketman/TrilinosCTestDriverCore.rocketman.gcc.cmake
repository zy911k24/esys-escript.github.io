# @HEADER
# ************************************************************************
#
#            Trilinos: An Object-Oriented Solver Framework
#                 Copyright (2001) Sandia Corporation
#
#
# Copyright (2001) Sandia Corporation. Under the terms of Contract
# DE-AC04-94AL85000, there is a non-exclusive license for use of this
# work by or on behalf of the U.S. Government.  Export of this program
# may require a license from the United States Government.
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the Corporation nor the names of the
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# NOTICE:  The United States Government is granted for itself and others
# acting on its behalf a paid-up, nonexclusive, irrevocable worldwide
# license in this data to reproduce, prepare derivative works, and
# perform publicly and display publicly.  Beginning five (5) years from
# July 25, 2001, the United States Government is granted for itself and
# others acting on its behalf a paid-up, nonexclusive, irrevocable
# worldwide license in this data to reproduce, prepare derivative works,
# distribute copies to the public, perform publicly and display
# publicly, and to permit others to do so.
#
# NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED STATES DEPARTMENT
# OF ENERGY, NOR SANDIA CORPORATION, NOR ANY OF THEIR EMPLOYEES, MAKES
# ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR
# RESPONSIBILITY FOR THE ACCURACY, COMPLETENESS, OR USEFULNESS OF ANY
# INFORMATION, APPARATUS, PRODUCT, OR PROCESS DISCLOSED, OR REPRESENTS
# THAT ITS USE WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.
#
# ************************************************************************
# @HEADER


INCLUDE("${CTEST_SCRIPT_DIRECTORY}/../../TrilinosCTestDriverCore.cmake")

#
# Platform/compiler specific options for rocketman using gcc
#

MACRO(TRILINOS_SYSTEM_SPECIFIC_CTEST_DRIVER)

  # Base of Trilinos/cmake/ctest then BUILD_DIR_NAME

  IF(COMM_TYPE STREQUAL MPI)
    string(TOUPPER $ENV{LMOD_FAMILY_MPI} UC_MPI_NAME)
    SET(BUILD_DIR_NAME ${UC_MPI_NAME}-$ENV{LMOD_FAMILY_MPI_VERSION}_${BUILD_TYPE}_${BUILD_NAME_DETAILS})
  ELSE()
    SET(BUILD_DIR_NAME ${COMM_TYPE}-${BUILD_TYPE}_${BUILD_NAME_DETAILS})
  ENDIF()

  SET(Trilinos_REPOSITORY_LOCATION_NIGHTLY_DEFAULT "https://github.com/muelu/Trilinos.git")

  SET(CTEST_DASHBOARD_ROOT  "${TRILINOS_CMAKE_DIR}/../../${BUILD_DIR_NAME}" )
  SET(CTEST_NOTES_FILES     "${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}" )

  SET_DEFAULT(Trilinos_ENABLE_SECONDARY_TESTED_CODE ON)
  SET(Trilinos_CTEST_DO_ALL_AT_ONCE ON)
  SET(Trilinos_SKIP_EXTRAREPOS_FILE TRUE)
  SET_DEFAULT(Trilinos_EXCLUDE_PACKAGES             ${EXTRA_EXCLUDE_PACKAGES} TriKota Optika)

  SET(EXTRA_SYSTEM_CONFIGURE_OPTIONS
    "-DBUILD_SHARED_LIBS=ON"
    "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
    "-DCMAKE_CXX_STANDARD=17"
    "-DCMAKE_VERBOSE_MAKEFILE=ON"

    "-DTrilinos_ENABLE_COMPLEX:BOOL=OFF"

    "-DTrilinos_ENABLE_TriKota:BOOL=OFF"
    "-DTrilinos_ENABLE_Optika:BOOL=OFF"

    "-DHDF5_INCLUDE_DIRS:FILEPATH=$ENV{HDF5_ROOT}/include"
    "-DHDF5_LIBRARY_DIRS:FILEPATH=$ENV{HDF5_ROOT}/lib"

    "-DSuperLU_INCLUDE_DIRS=$ENV{SUPERLU_INC}"
    "-DSuperLU_LIBRARY_DIRS=$ENV{SUPERLU_LIB}"

    "-DBoost_INCLUDE_DIRS:STRING=$ENV{BOOST_INC}"
    "-DBoost_LIBRARY_DIRS:STRING=$ENV{BOOST_LIB}"
    "-DBoostLib_INCLUDE_DIRS:STRING=$ENV{BOOST_INC}"
    "-DBoostLib_LIBRARY_DIRS:STRING=$ENV{BOOST_LIB}"

    "-DNetcdf_LIBRARY_DIRS:STRING=$ENV{NETCDF_C_LIB}"
    "-DNetcdf_INCLUDE_DIRS:STRING=$ENV{NETCDF_C_INC}"

    ### PACKAGE CONFIGURATION ###

    ### MISC ###
    "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
    )

  SET_DEFAULT(COMPILER_VERSION "$ENV{LMOD_FAMILY_COMPILER}-$ENV{LMOD_FAMILY_COMPILER_VERSION}")

  # Ensure that MPI is on for all parallel builds that might be run.
  IF(COMM_TYPE STREQUAL MPI)

    SET(EXTRA_SYSTEM_CONFIGURE_OPTIONS
        ${EXTRA_SYSTEM_CONFIGURE_OPTIONS}
        "-DTPL_ENABLE_MPI=ON"
            "-DMPI_BASE_DIR:PATH=$ENV{OPENMPI_ROOT}"
        "-DTPL_BLAS_LIBRARIES=/usr/lib64/libblas.so"
        "-DTPL_LAPACK_LIBRARIES=/usr/lib64/liblapack.so"
       )

    #NUMA memory binding doesn't work on rocketman currently; TODO: get libnumactl and enable binding again
    #"-DMPI_EXEC_POST_NUMPROCS_FLAGS:STRING=--bind-to\\\;core\\\;--map-by\\\;core"

    SET(CTEST_MEMORYCHECK_COMMAND_OPTIONS
        "--gen-suppressions=all --error-limit=no --log-file=nightly_suppressions.txt" ${CTEST_MEMORYCHECK_COMMAND_OPTIONS} )

  ELSE()

    SET( EXTRA_SYSTEM_CONFIGURE_OPTIONS
      ${EXTRA_SYSTEM_CONFIGURE_OPTIONS}
      "-DCMAKE_CXX_COMPILER=$ENV{GCC_ROOT}/bin/g++"
      "-DCMAKE_C_COMPILER=$ENV{GCC_ROOT}/bin/gcc"
      )

  ENDIF()

  TRILINOS_CTEST_DRIVER()

ENDMACRO()