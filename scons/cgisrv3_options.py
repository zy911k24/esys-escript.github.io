
##############################################################################
#
# Copyright (c) 2003-2018 by The University of Queensland
# http://www.uq.edu.au
#
# Primary Business: Queensland, Australia
# Licensed under the Apache License, version 2.0
# http://www.apache.org/licenses/LICENSE-2.0
#
# Development until 2012 by Earth Systems Science Computational Center (ESSCC)
# Development 2012-2013 by School of Earth Sciences
# Development from 2014 by Centre for Geoscience Computing (GeoComp)
#
##############################################################################

import os

# PREFIXES:
# There are two ways to specify where to find dependent headers and libraries
# (via the <dependency>_prefix):
# 1) If your installation follows the general scheme where headers are located
#    in <prefix>/include[32,64], and libraries in <prefix>/lib[32,64] then
#    it is sufficient to specify this prefix, e.g. boost_prefix='C:/python'
# 2) Otherwise provide a list with two elements, where the first one is the
#    include path, and the second the library path, e.g.
#    boost_prefix=['C:/boost/include/boost1_44', 'C:/boost/lib']
# All <dependency>_prefix settings default to '/usr' so have to be set
# manually on Windows.

# The options file version. SCons will refuse to build if there have been
# changes to the set of variables and your file has not been updated.
# This setting is mandatory.
escript_opts_version = 201

# Installation prefix. Files will be installed in subdirectories underneath.
# DEFAULT: '.' (current directory)
#prefix = 'C:/escript'

# Top-level directory for intermediate build and test files.
# DEFAULT: 'build'
#build_dir = 'build'

# C compiler command name or full path.
# DEFAULT: auto-detected
#cc = 'cl'

# C++ compiler command name or full path.
# DEFAULT: auto-detected
#cxx = 'cl'

# Flags to use with both C and C++ compilers. Do not set unless you know
# what you are doing - use cc_extra to specify additional flags!
# DEFAULT: compiler-dependent
#cc_flags = ''

# Additional compiler (optimization) flags for non-debug builds
# DEFAULT: compiler-dependent
#cc_optim = '/O2 /Op /W3'

# Additional compiler flags for debug builds
# DEFAULT: compiler-dependent
#cc_debug = '/Od /RTCcsu /ZI /DBOUNDS_CHECK'

# Additional flags to add to the C compiler only
# DEFAULT: '' (empty)
#cc_extra = ''

# Additional flags to add to the C++ compiler only
# DEFAULT: '' (empty)
#cxx_extra = ''

# Additional flags to add to the linker
# DEFAULT: '' (empty)
#ld_extra = ''

# Whether to treat compiler warnings as errors
# DEFAULT: True
werror = False

# Whether to build a debug version
# DEFAULT: False
#debug = True

# Set to True to print the full compiler/linker command line
# DEFAULT: False
verbose = True

# Set to True to add flags that enable OpenMP parallelization
# DEFAULT: False
#openmp = True

# Additional compiler flags for OpenMP builds
# DEFAULT: compiler-dependent
#omp_flags = '/Qopenmp /Qparallel'

# Additional linker flags for OpenMP builds
# DEFAULT: compiler-dependent
#omp_ldflags = '/Qopenmp /Qparallel'

# Flavour of MPI implementation
# Recognized values: 'none', 'MPT', 'MPICH', 'MPICH2', 'OPENMPI', 'INTELMPI'
# DEFAULT: 'none' (disable MPI)
mpi = 'MPICH2'

# Prefix or paths to MPI headers and libraries. See note above about prefixes.
mpi_prefix = 'C:/Program Files/MPICH2'

# MPI libraries to link against
mpi_libs = ['mpi']

dotdot = os.path.realpath('..')
system_builddeps = os.path.join('c:',os.sep, 'buildlibs')

# Prefix or paths to python headers and libraries. See note above.
# By default, this is determined using the running python executable.
#python_prefix = 'C:/Python26'

# Prefix or paths to boost-python headers and libraries. See note above.
boost_prefix = [os.path.join(system_builddeps, 'boost_1_41_0'), os.path.join(system_builddeps, 'boost_1_41_0','windows_binary','lib')]

# boost-python library/libraries to link against
boost_libs = ['boost_python-vc71-mt-1_41']

# Prefix or paths to CppUnit headers and libraries. See note above.
# C:\buildlibs\cppunit-1.12.1\src\cppunit\Release
cppunit_prefix = os.path.join(system_builddeps, 'cppunit-1.12.1')

# CppUnit library/libraries to link against
cppunit_libs = ['cppunit']

# Whether to use the netCDF library for dump file support
# DEFAULT: False
netcdf = True

# Prefix or paths to netCDF headers and libraries. See note above.
netcdf_prefix = [os.path.join(system_builddeps, 'netcdf', 'src', 'include'), os.path.join(system_builddeps, 'netcdf', 'lib')]

# netCDF library/libraries to link against
netcdf_libs = ['netcdf_cpp', 'netcdf']

# Whether to use the parMETIS library (only in conjunction with MPI)
# DEFAULT: False
#parmetis = True

# Prefix or paths to parMETIS headers and libraries. See note above.
#parmetis_prefix = 'C:/parmetis'

# parMETIS library/libraries to link against
#parmetis_libs = ['parmetis', 'metis']

# Whether to use the Intel PAPI (Performance API) library
# DEFAULT: False
#papi = True

# Prefix or paths to PAPI headers and libraries. See note above.
#papi_prefix = 'C:/papi'

# PAPI library/libraries to link against
#papi_libs = ['papi']

# Whether to use PAPI to instrument solver iterations
# DEFAULT: False
#papi_instrument_solver = True

# Whether to use Intel MKL (Math Kernel Library)
# DEFAULT: False
#mkl = True

# Prefix or paths to MKL headers and libraries. See note above.
#mkl_prefix = 'C:/mkl'

# MKL library/libraries to link against
#mkl_libs = ['mkl_solver', 'mkl_em64t', 'mkl_core', 'guide']

# Whether to use UMFPACK (requires AMD and BLAS)
# DEFAULT: False
#umfpack = True

# Prefix or paths to UMFPACK headers and libraries. See note above.
#umfpack_prefix = 'C:/umfpack'

# UMFPACK library/libraries to link against
#umfpack_libs = ['umfpack']

# Whether to use BoomerAMG (requires MPI)
# DEFAULT: False
#boomeramg = True

# Prefix or paths to BoomerAMG headers and libraries. See note above.
#boomeramg_prefix = 'C:/boomeramg'

# BoomerAMG library/libraries to link against
#boomeramg_libs = ['HYPRE']

# Flavour of LAPACK implementation
# Recognized values: 'none', 'clapack', 'mkl'
# DEFAULT: 'none' (do not use LAPACK)
#lapack = 'clapack'

# Prefix or paths to LAPACK headers and libraries. See note above.
#lapack_prefix = 'C:/lapack'

# LAPACK library/libraries to link against
#lapack_libs = ['lapack_atlas']

# Whether to use LLNL's SILO library for Silo output file support in weipa
# DEFAULT: False
#silo = True

# Prefix or paths to SILO headers and libraries. See note above.
#silo_prefix = 'C:/silo'

# SILO library/libraries to link against
#silo_libs = ['siloh5', 'hdf5']

# Whether to use LLNL's VisIt simulation interface (only version 2 supported)
# DEFAULT: False
#visit = True

# Prefix or paths to VisIt's sim2 headers and libraries. See note above.
#visit_prefix = 'C:/visit/2.1.0/linux-intel/libsim/V2'

# Sim2 library/libraries to link against
#visit_libs = ['simV2']

### ADVANCED OPTIONS ###
# Do not change the following options unless you know what they do

# Use intel's VSL library for random data
# DEFAULT: False
#vsl_random = True

# Extra libraries to link with
sys_libs = ['C:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/PlatformSDK/Lib/Ws2_32']

# Additional environmental variables to export to the tools
#env_export = []

#tools_names = ['msvc']

#iknowwhatimdoing = False

#forcelazy = 'leave_alone'

#forcecollres = 'leave_alone'

