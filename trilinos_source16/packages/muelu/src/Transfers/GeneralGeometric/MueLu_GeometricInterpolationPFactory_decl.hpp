// @HEADER
// *****************************************************************************
//        MueLu: A package for multigrid based preconditioning
//
// Copyright 2012 NTESS and the MueLu contributors.
// SPDX-License-Identifier: BSD-3-Clause
// *****************************************************************************
// @HEADER

#ifndef MUELU_GEOMETRICINTERPOLATIONPFACTORY_DECL_HPP
#define MUELU_GEOMETRICINTERPOLATIONPFACTORY_DECL_HPP

// Teuchos includes for dense linear algebra
#include <Teuchos_SerialDenseMatrix.hpp>
#include <Teuchos_SerialDenseVector.hpp>
#include <Teuchos_SerialDenseSolver.hpp>

#include "Xpetra_CrsGraph_fwd.hpp"

#include "MueLu_PFactory.hpp"
#include "MueLu_Level_fwd.hpp"
#include "MueLu_Aggregates_fwd.hpp"

namespace MueLu {

template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node>
class GeometricInterpolationPFactory : public PFactory {
#undef MUELU_GEOMETRICINTERPOLATIONPFACTORY_SHORT
#include "MueLu_UseShortNames.hpp"

 public:
  // Declare useful types
  using real_type                  = typename Teuchos::ScalarTraits<SC>::coordinateType;
  using realvaluedmultivector_type = Xpetra::MultiVector<real_type, LO, GO, Node>;

  //! @name Constructors/Destructors.
  //@{

  //! Constructor
  GeometricInterpolationPFactory() {}

  //! Destructor.
  virtual ~GeometricInterpolationPFactory() {}
  //@}

  RCP<const ParameterList> GetValidParameterList() const;

  //! Input
  //@{

  void DeclareInput(Level& fineLevel, Level& coarseLevel) const;

  //@}

  //! @name Build methods.
  //@{

  void Build(Level& fineLevel, Level& coarseLevel) const;
  void BuildP(Level& fineLevel, Level& coarseLevel) const;

  //@}

 private:
  void BuildConstantP(RCP<Matrix>& P, RCP<const CrsGraph>& prolongatorGraph, RCP<Matrix>& A) const;
  void BuildLinearP(Level& coarseLevel,
                    RCP<Matrix>& A, RCP<const CrsGraph>& prolongatorGraph,
                    RCP<realvaluedmultivector_type>& fineCoordinates,
                    RCP<realvaluedmultivector_type>& ghostCoordinates,
                    const int numDimensions, const bool keepD2,
                    RCP<Matrix>& P) const;
  void ComputeLinearInterpolationStencil(const int numDimensions, const int numInterpolationPoints,
                                         const Array<Array<real_type> > coord,
                                         Array<real_type>& stencil) const;
  void GetInterpolationFunctions(const LO numDimensions,
                                 const Teuchos::SerialDenseVector<LO, real_type> parametricCoordinates,
                                 real_type functions[4][8]) const;

};  // class GeometricInterpolationPFactory

}  // namespace MueLu

#define MUELU_GEOMETRICINTERPOLATIONPFACTORY_SHORT
#endif  // MUELU_GEOMETRICINTERPOLATIONPFACTORY_DECL_HPP