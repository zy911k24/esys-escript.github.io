// @HEADER
// *****************************************************************************
//        MueLu: A package for multigrid based preconditioning
//
// Copyright 2012 NTESS and the MueLu contributors.
// SPDX-License-Identifier: BSD-3-Clause
// *****************************************************************************
// @HEADER

#ifndef MUELU_SIMPLESMOOTHER_DECL_HPP_
#define MUELU_SIMPLESMOOTHER_DECL_HPP_

#include "MueLu_ConfigDefs.hpp"

#include <Teuchos_ParameterList.hpp>

// Xpetra
#include <Xpetra_MapExtractor_fwd.hpp>
#include <Xpetra_MultiVectorFactory_fwd.hpp>
#include <Xpetra_Matrix_fwd.hpp>

// MueLu
#include "MueLu_SimpleSmoother_fwd.hpp"
#include "MueLu_SmootherPrototype.hpp"
#include "MueLu_FactoryBase_fwd.hpp"
#include "MueLu_FactoryManagerBase_fwd.hpp"
#include "MueLu_SmootherBase_fwd.hpp"
#include "MueLu_Utilities_fwd.hpp"

#include "MueLu_SchurComplementFactory_fwd.hpp"
#include "MueLu_FactoryManager_fwd.hpp"

namespace MueLu {

/*!
  @class SimpleSmoother
  @brief SIMPLE smoother for 2x2 block matrices

*/

template <class Scalar        = SmootherPrototype<>::scalar_type,
          class LocalOrdinal  = typename SmootherPrototype<Scalar>::local_ordinal_type,
          class GlobalOrdinal = typename SmootherPrototype<Scalar, LocalOrdinal>::global_ordinal_type,
          class Node          = typename SmootherPrototype<Scalar, LocalOrdinal, GlobalOrdinal>::node_type>
class SimpleSmoother : public SmootherPrototype<Scalar, LocalOrdinal, GlobalOrdinal, Node> {
  typedef Xpetra::MapExtractor<Scalar, LocalOrdinal, GlobalOrdinal, Node> MapExtractorClass;

#undef MUELU_SIMPLESMOOTHER_SHORT
#include "MueLu_UseShortNames.hpp"

 public:
  //! @name Constructors / destructors
  //@{

  //! Constructor
  SimpleSmoother();

  //! Destructor
  virtual ~SimpleSmoother();

  //@}

  //! Input
  //@{

  RCP<const ParameterList> GetValidParameterList() const;

  void DeclareInput(Level &currentLevel) const;

  //! Set factory manager for internal velocity prediction
  //! This routine is outdated. Use AddFactoryManager instead
  void SetVelocityPredictionFactoryManager(RCP<FactoryManager> FactManager);

  //! Set factory manager for internal SchurComplement handling
  //! This routine is outdated. Use AddFactoryManager instead
  void SetSchurCompFactoryManager(RCP<FactoryManager> FactManager);

  //! Add a factory manager at a specific position
  void AddFactoryManager(RCP<const FactoryManagerBase> FactManager, int pos);

  //@}

  //! @name Setup and Apply methods.
  //@{

  /*!
    @brief Setup routine

    The Setup() routine can be summarized in 4 steps:
    1. Set the map extractors
    2. Set the blocks
    3. Create and set the inverse of the diagonal of F
    4. Set the smoother for the Schur Complement
   */
  void Setup(Level &currentLevel);

  /*! @brief Apply the Braess Sarazin smoother.
  @param X initial guess
  @param B right-hand side
  @param InitialGuessIsZero TODO This option has no effect.
  */
  void Apply(MultiVector &X, MultiVector const &B, bool InitialGuessIsZero = false) const;
  //@}

  RCP<SmootherPrototype> Copy() const;

  //! @name Overridden from Teuchos::Describable
  //@{

  //! Return a simple one-line description of this object.
  std::string description() const;

  //! Print the object with some verbosity level to an FancyOStream object.
  // using MueLu::Describable::describe; // overloading, not hiding
  void print(Teuchos::FancyOStream &out, const VerbLevel verbLevel = Default) const;

  //! Get a rough estimate of cost per iteration
  size_t getNodeSmootherComplexity() const;

  //@}

 private:
  //! smoother type
  std::string type_;

  //!< Generating factory for operator "A"
  RCP<const FactoryBase> AFact_;

  //! Internal blocked operator "A" generated by #AFact_
  RCP<Matrix> A_;

  //! Range  map extractor (from A_ generated by AFact)
  RCP<const MapExtractorClass> rangeMapExtractor_;

  //! Domain map extractor (from A_ generated by AFact)
  RCP<const MapExtractorClass> domainMapExtractor_;

  //! @group Matrices
  //! @{

  //! Inverse diagonal of fluid operator (vector).
  Teuchos::RCP<Vector> diagFinv_;

  //! Fluid operator
  Teuchos::RCP<Matrix> F_;

  //! Pressure gradient operator
  Teuchos::RCP<Matrix> G_;

  //! Divergence operator
  Teuchos::RCP<Matrix> D_;

  //! Pressure stabilization term or null block
  Teuchos::RCP<Matrix> Z_;

  //! @}

  //! Smoother for velocity prediction
  Teuchos::RCP<SmootherBase> velPredictSmoo_;

  //! Smoother for SchurComplement equation
  Teuchos::RCP<SmootherBase> schurCompSmoo_;

  /*! @brief Vector of internal factory managers
   *
   * - FactManager_[0] holds the factory manager for the prediction of the primary variable
   * - FactManager_[1] stores the factory manager used for the SchurComplement correction step.
   */
  std::vector<Teuchos::RCP<const FactoryManagerBase> > FactManager_;
};

}  // namespace MueLu

#define MUELU_SIMPLESMOOTHER_SHORT

#endif /* MUELU_SIMPLESMOOTHER_DECL_HPP_ */