
##############################################################################
#
# Copyright (c) 2003-2020 by The University of Queensland
# http://www.uq.edu.au
#
# Primary Business: Queensland, Australia
# Licensed under the Apache License, version 2.0
# http://www.apache.org/licenses/LICENSE-2.0
#
# Development until 2012 by Earth Systems Science Computational Center (ESSCC)
# Development 2012-2013 by School of Earth Sciences
# Development from 2014 by Centre for Geoscience Computing (GeoComp)
# Development from 2019 by School of Earth and Environmental Sciences
#
##############################################################################

# This is a template configuration file for escript on OpenSUSE.
# Refer to README_FIRST for usage instructions.

escript_opts_version = 203
openmp = True
boost_libs = ['boost_python-py3']
pythoncmd = '/usr/bin/python3'
pythonlibpath = ['/usr/lib64']
pythonlibname = ['python3.6m']
pythonincpath = ['/usr/include/python3.6m']
netcdf = True
mumps_libs=['mumps_common','cmumps_seq','dmumps_seq','zmumps_seq','zmumps_seq','pord']
umfpack = True
umfpack_prefix = ['/usr/include/suitesparse', '/usr/lib64']
umfpack_libs = ['umfpack', 'amd']
use_gmsh=0