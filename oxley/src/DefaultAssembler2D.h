
/*****************************************************************************
*
* Copyright (c) 2003-2018 by The University of Queensland
* http://www.uq.edu.au
*
* Primary Business: Queensland, Australia
* Licensed under the Apache License, version 2.0
* http://www.apache.org/licenses/LICENSE-2.0
*
* Development until 2012 by Earth Systems Science Computational Center (ESSCC)
* Development 2012-2013 by School of Earth Sciences
* Development from 2014 by Centre for Geoscience Computing (GeoComp)
*
*****************************************************************************/
#ifndef __OXLEY_DEFAULTASSEMBLER2D_H__
#define __OXLEY_DEFAULTASSEMBLER2D_H__

#include <oxley/Rectangle.h>

namespace oxley {

template<class Scalar = double>
class DefaultAssembler2D : public AbstractAssembler
{
public:
    DefaultAssembler2D<Scalar>(escript::const_Domain_ptr dom)
        : AbstractAssembler()
    {
        domain = REFCOUNTNS::static_pointer_cast<const Rectangle>(dom);
    }

    ~DefaultAssembler2D() {}

    /* The default OxleyDomain assemblers, with original signatures */
    
    /// assembles a single PDE into the system matrix 'mat' and the right hand
    /// side 'rhs'
    virtual void assemblePDESingle(escript::AbstractSystemMatrix* mat, escript::Data& rhs,
            const escript::Data& A, const escript::Data& B,
            const escript::Data& C, const escript::Data& D,
            const escript::Data& X, const escript::Data& Y) const;

    /// assembles boundary conditions of a single PDE into the system matrix
    /// 'mat' and the right hand side 'rhs'
    virtual void assemblePDEBoundarySingle(escript::AbstractSystemMatrix* mat,
            escript::Data& rhs, const escript::Data& d,
            const escript::Data& y) const;

    /// assembles a single PDE with reduced order into the system matrix 'mat'
    /// and the right hand side 'rhs'
    virtual void assemblePDESingleReduced(escript::AbstractSystemMatrix* mat,
            escript::Data& rhs, const escript::Data& A, const escript::Data& B,
            const escript::Data& C, const escript::Data& D,
            const escript::Data& X, const escript::Data& Y) const;

    /// assembles boundary conditions of a single PDE with reduced order into
    /// the system matrix 'mat' and the right hand side 'rhs'
    virtual void assemblePDEBoundarySingleReduced(escript::AbstractSystemMatrix* mat,
            escript::Data& rhs, const escript::Data& d,
            const escript::Data& y) const;

    /// assembles a system of PDEs into the system matrix 'mat' and the right
    /// hand side 'rhs'
    virtual void assemblePDESystem(escript::AbstractSystemMatrix* mat, escript::Data& rhs,
            const escript::Data& A, const escript::Data& B,
            const escript::Data& C, const escript::Data& D,
            const escript::Data& X, const escript::Data& Y) const;

    /// assembles boundary conditions of a system of PDEs into the system
    /// matrix 'mat' and the right hand side 'rhs'
    virtual void assemblePDEBoundarySystem(escript::AbstractSystemMatrix* mat,
            escript::Data& rhs, const escript::Data& d,
            const escript::Data& y) const;

    /// assembles a system of PDEs with reduced order into the system matrix
    /// 'mat' and the right hand side 'rhs'
    virtual void assemblePDESystemReduced(escript::AbstractSystemMatrix* mat,
            escript::Data& rhs, const escript::Data& A, const escript::Data& B,
            const escript::Data& C, const escript::Data& D,
            const escript::Data& X, const escript::Data& Y) const;

    /// assembles boundary conditions of a system of PDEs with reduced order
    /// into the system matrix 'mat' and the right hand side 'rhs'
    virtual void assemblePDEBoundarySystemReduced(escript::AbstractSystemMatrix* mat,
            escript::Data& rhs, const escript::Data& d,
            const escript::Data& y) const;
    
    /* The new interface for assemblers */

    virtual void assemblePDESingle(escript::AbstractSystemMatrix* mat,
                                   escript::Data& rhs,
                                   const DataMap& coefs) const;
    virtual void assemblePDEBoundarySingle(escript::AbstractSystemMatrix* mat,
                                           escript::Data& rhs,
                                           const DataMap& coefs) const;
    virtual void assemblePDESingleReduced(escript::AbstractSystemMatrix* mat,
                                          escript::Data& rhs,
                                          const DataMap& coefs) const;
    virtual void assemblePDEBoundarySingleReduced(
                                          escript::AbstractSystemMatrix* mat,
                                          escript::Data& rhs,
                                          const DataMap& coefs) const;
    virtual void assemblePDESystem(escript::AbstractSystemMatrix* mat,
                                   escript::Data& rhs,
                                   const DataMap& coefs) const;
    virtual void assemblePDEBoundarySystem(escript::AbstractSystemMatrix* mat,
                                           escript::Data& rhs,
                                           const DataMap& coefs) const;
    virtual void assemblePDESystemReduced(escript::AbstractSystemMatrix* mat,
                                          escript::Data& rhs,
                                          const DataMap& coefs) const;
    virtual void assemblePDEBoundarySystemReduced(
                                          escript::AbstractSystemMatrix* mat,
                                          escript::Data& rhs,
                                          const DataMap& coefs) const;

    virtual void collateFunctionSpaceTypes(std::vector<int>& fsTypes,
                                           const DataMap& coefs) const;

protected:
    POINTER_WRAPPER_CLASS(const Rectangle) domain;
    // const double *m_dx;
    // const dim_t *m_NE;
    // const dim_t *m_NN;
};

} // namespace oxley

#endif // __OXLEY_DEFAULTASSEMBLER2D_H__
