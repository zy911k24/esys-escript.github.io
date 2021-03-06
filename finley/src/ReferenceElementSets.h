
/*****************************************************************************
*
* Copyright (c) 2003-2020 by The University of Queensland
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

#ifndef __FINLEY_REFERENCEELEMENTSETS_H__
#define __FINLEY_REFERENCEELEMENTSETS_H__

#include "ReferenceElements.h"

namespace finley {

///  A reference element set manages the reference elements for the full
///  and reduced integration order
struct ReferenceElementSet {
    ReferenceElementSet(ElementTypeId id, int order, int reduced_order)
    {
        const ReferenceElementInfo* id_info = ReferenceElement::getInfo(id);
        const ShapeFunctionInfo* bf_info = ShapeFunction::getInfo(
                                                    id_info->BasisFunctions);
        if (order<0)
            order=std::max(2*bf_info->numOrder, 0);

        referenceElement.reset(new ReferenceElement(id, order));
        if (reduced_order<0)
            reduced_order=std::max(2*(bf_info->numOrder-1), 0);
        referenceElementReducedQuadrature.reset(new ReferenceElement(id,
                                                             reduced_order));

        if (referenceElement->getNumNodes() != referenceElementReducedQuadrature->getNumNodes()) {
            throw escript::ValueError("ReferenceElementSet: numNodes in referenceElement and referenceElementReducedQuadrature don't match.");
        }
    }

    const_ShapeFunction_ptr borrowBasisFunctions(bool reducedShapefunction,
                                                 bool reducedIntegrationOrder) const
    {
        if (reducedShapefunction) {
            return (reducedIntegrationOrder ?
                    referenceElementReducedQuadrature->LinearBasisFunctions :
                    referenceElement->LinearBasisFunctions);
        }
        return (reducedIntegrationOrder ?
                referenceElementReducedQuadrature->BasisFunctions :
                referenceElement->BasisFunctions);
    }

    const_ShapeFunction_ptr borrowParametrization(bool reducedIntegrationOrder) const
    {
        return (reducedIntegrationOrder ?
                referenceElementReducedQuadrature->Parametrization :
                referenceElement->Parametrization);
    }

    const_ReferenceElement_ptr borrowReferenceElement(bool reducedIntOrder) const
    {
        return (reducedIntOrder ? referenceElementReducedQuadrature :
                                  referenceElement);
    }

    inline int getNumNodes() const { return referenceElement->getNumNodes(); }

    ReferenceElement_ptr referenceElementReducedQuadrature;
    ReferenceElement_ptr referenceElement;
};


typedef boost::shared_ptr<const ReferenceElementSet> const_ReferenceElementSet_ptr;


} // namespace finley

#endif // __FINLEY_REFERENCEELEMENTSETS_H__

