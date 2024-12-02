
/*****************************************************************************
*
* Copyright (c) 2016 by The University of Queensland
* http://www.uq.edu.au
*
* Primary Business: Queensland, Australia
* Licensed under the Apache License, version 2.0
* http://www.apache.org/licenses/LICENSE-2.0
*
* Development until 2012 by Earth Systems Science Computational Center (ESSCC)
* Development 2012-2013 by School of Earth Sciences
* Development from 2014-2017 by Centre for Geoscience Computing (GeoComp)
* Development from 2019 by School of Earth and Environmental Sciences
**
*****************************************************************************/

#ifndef __ESYS_TRILINOS_PRECONDITIONERFACTORY_H__
#define __ESYS_TRILINOS_PRECONDITIONERFACTORY_H__

#include <trilinoswrap/types.h>

/// Wrapper for Ifpack2 and MueLu

namespace escript {
    class SolverBuddy;
}

namespace esys_trilinos {

/// creates a preconditioner (Operator) for input matrix A using options in sb.
/// ST is the scalar type used by the matrix.
template<typename ST>
Teuchos::RCP<OpType<ST> > createPreconditioner(
                                      Teuchos::RCP<const MatrixType<ST> > A,
                                      const escript::SolverBuddy& sb);

} // namespace esys_trilinos

#endif // __ESYS_TRILINOS_PRECONDITIONERFACTORY_H__

