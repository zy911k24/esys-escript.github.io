
##############################################################################
#
# Copyright (c) 2003-2015 by The University of Queensland
# http://www.uq.edu.au
#
# Primary Business: Queensland, Australia
# Licensed under the Open Software License version 3.0
# http://www.opensource.org/licenses/osl-3.0.php
#
# Development until 2012 by Earth Systems Science Computational Center (ESSCC)
# Development 2012-2013 by School of Earth Sciences
# Development from 2014 by Centre for Geoscience Computing (GeoComp)
#
##############################################################################

"""Collection of forward models that define the inversion problem"""

__copyright__="""Copyright (c) 2003-2015 by The University of Queensland
http://www.uq.edu.au
Primary Business: Queensland, Australia"""
__license__="""Licensed under the Open Software License version 3.0
http://www.opensource.org/licenses/osl-3.0.php"""
__url__="https://launchpad.net/escript-finley"

from .base import *
from .acoustic import *
from .dcresistivity import *
from .gravity import *
from .magnetic import *
from .magnetotelluric2d import *
from .pressure import *
from .subsidence import *
