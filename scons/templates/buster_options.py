
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

# This is a template configuration file for escript on Debian/GNU Linux.
# Refer to README_FIRST for usage instructions.

escript_opts_version = 203
#cxx_extra = '-Wno-literal-suffix'
openmp = True
#mpi = 'OPENMPI'

import os

mpi_prefix=['/usr/lib/x86_64-linux-gnu/openmpi/include/','/usr/lib/x86_64-linux-gnu/openmpi/lib/']
mpi_libs = ['mpi_cxx', 'mpi', 'open-rte', 'open-pal']
netcdf = 4
umfpack = True
umfpack_prefix = ['/usr/include/suitesparse', '/usr/lib']
umfpack_libs = ['umfpack', 'blas', 'amd']
lapack_prefix = ['/usr/include/x86_64-linux-gnu/', '/usr/lib/x86_64-linux-gnu/']
#silo = True
silo_libs = ['siloh5', 'hdf5_openmpi']
dudley_assemble_flags = '-funroll-loops'
pythoncmd="/usr/bin/python3"
pythonlibname = 'python3.7m'
pythonlibpath = '/usr/lib/x86_64-linux-gnu/'
pythonincpath = '/usr/include/python3.7m'

import subprocess
import os
p = subprocess.Popen(["ld","--verbose"], stdout=subprocess.PIPE)
out,err = p.communicate()
spath = [x[13:-3] for x in out.decode().split() if 'SEARCH_DIR' in x]
p2name = ''
p3name = ''
for name in spath:
  try:
    l=os.listdir(name)
    p2res=[x for x in l if x.startswith('libboost_python2') and x.endswith('.so')]
    p3res=[x for x in l if x.startswith('libboost_python3') and x.endswith('.so')]
    if len(p2name)==0 and len(p2res)>0:
      p2name=p2res[-1]
    if len(p3name)==0 and len(p3res)>0:
      p3name=p3res[-1]
  except OSError:
    pass


# boost-python library/libraries to link against
boost_libs = [p3name[3:-3]]

# this can be used by options files importing us
boost_py2_libs = [p2name[3:-3]]
boost_py3_libs = [p3name[3:-3]]

from site_init import getdebbuildflags
# Now we add the debian build flags
debstuff = getdebbuildflags()
if len(debstuff) > 0:
  print("Building with the following additional flags from debian: "+str(debstuff))
for i in debstuff:
  k=i[0]
  v=i[1]
  try:
    exec(k+"+=' "+v+"'")
  except NameError:   
    exec(k+"='"+v+"'")

mathjax_path='/usr/share/javascript/mathjax/MathJax.js'

mumps_libs=['cmumps','dmumps','smumps','zmumps','mumps_common','lapack','metis']
mumps_prefix=['/usr/include','/usr/lib/x86_64-linux-gnu/']
