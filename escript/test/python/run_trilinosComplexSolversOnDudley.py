
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

from __future__ import print_function, division

__copyright__="""Copyright (c) 2003-2018 by The University of Queensland
http://www.uq.edu.au
Primary Business: Queensland, Australia"""
__license__="""Licensed under the Open Software License version 3.0
http://www.opensource.org/licenses/osl-3.0.php"""
__url__="https://launchpad.net/escript-finley"

"""
Test suite for PDE solvers on ripley
"""

from test_simplesolve import ComplexSolveTestCase
import esys.escriptcore.utestselect as unittest
from esys.escriptcore.testing import *

from esys.escript import getMPISizeWorld, hasFeature, sqrt
from esys.dudley import Rectangle, Brick
from esys.escript.linearPDEs import SolverOptions

HAVE_TRILINOS = hasFeature('trilinos')
skip_muelu_long = False #hasFeature("longindex")



# number of elements in the spatial directions
NE0=12
NE1=12
NE2=8
OPTIMIZE=True


@unittest.skipIf(not HAVE_TRILINOS, "Trilinos not available")
class ComplexSolveOnTrilinos(ComplexSolveTestCase):
    pass


## direct
class Test_ComplexSolveDudley2D_Trilinos_Direct(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Rectangle(NE0, NE1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.DIRECT

    def tearDown(self):
        del self.domain

class Test_ComplexSolveDudley3D_Trilinos_Direct(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Brick(NE0, NE1, NE2, 1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.DIRECT

    def tearDown(self):
        del self.domain
### BiCGStab + Jacobi

class Test_ComplexSolveDudley2D_Trilinos_BICGSTAB_Jacobi(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Rectangle(NE0, NE1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.BICGSTAB
        self.preconditioner = SolverOptions.JACOBI

    def tearDown(self):
        del self.domain

@unittest.skip("fails with Nan during iteration.")
class Test_ComplexSolveDudley3D_Trilinos_BICGSTAB_Jacobi(ComplexSolveOnTrilinos):
    SOLVER_TOL = 1.e-9
    def setUp(self):
        self.domain = Brick(NE0, NE1, NE2, 1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.BICGSTAB
        self.preconditioner = SolverOptions.JACOBI

    def tearDown(self):
        del self.domain

### GMRES + Jacobi

class Test_ComplexSolveDudley2D_Trilinos_GMRES_Jacobi(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Rectangle(NE0, NE1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.GMRES
        self.preconditioner = SolverOptions.JACOBI

    def tearDown(self):
        del self.domain

class Test_ComplexSolveDudley3D_Trilinos_GMRES_Jacobi(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Brick(NE0, NE1, NE2, 1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.GMRES
        self.preconditioner = SolverOptions.JACOBI

    def tearDown(self):
        del self.domain

### PCG + Jacobi

class Test_ComplexSolveDudley2D_Trilinos_PCG_Jacobi(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Rectangle(NE0, NE1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.PCG
        self.preconditioner = SolverOptions.JACOBI

    def tearDown(self):
        del self.domain

class Test_ComplexSolveDudley3D_Trilinos_PCG_Jacobi(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Brick(NE0, NE1, NE2, 1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.PCG
        self.preconditioner = SolverOptions.JACOBI

    def tearDown(self):
        del self.domain

### PCG + AMG

@unittest.skipIf(skip_muelu_long, "MueLu AMG incompatible with index type long")
class Test_ComplexSolveDudley2D_Trilinos_PCG_AMG(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Rectangle(NE0, NE1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.PCG
        self.preconditioner = SolverOptions.AMG

    def _setSolverOptions(self, so):
        so.setTrilinosParameter("number of equations", 2)

    def tearDown(self):
        del self.domain

@unittest.skipIf(skip_muelu_long, "MueLu AMG incompatible with index type long")
class Test_ComplexSolveDudley3D_Trilinos_PCG_AMG(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Brick(NE0, NE1, NE2, 1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.PCG
        self.preconditioner = SolverOptions.AMG

    def _setSolverOptions(self, so):
        so.setTrilinosParameter("number of equations", 3)

    def tearDown(self):
        del self.domain

### PCG + ILUT

class Test_ComplexSolveDudley2D_Trilinos_PCG_ILUT(ComplexSolveOnTrilinos):
    def setUp(self):
        self.domain = Rectangle(NE0, NE1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.PCG
        self.preconditioner = SolverOptions.ILUT

    def tearDown(self):
        del self.domain

class Test_ComplexSolveDudley3D_Trilinos_PCG_ILUT(ComplexSolveOnTrilinos):
    SOLVER_TOL = 1.e-9
    def setUp(self):
        self.domain = Brick(NE0, NE1, NE2, 1, optimize=OPTIMIZE)
        self.package = SolverOptions.TRILINOS
        self.method = SolverOptions.PCG
        self.preconditioner = SolverOptions.ILUT

    def tearDown(self):
        del self.domain


if __name__ == '__main__':
   run_tests(__name__, exit_on_failure=True)
