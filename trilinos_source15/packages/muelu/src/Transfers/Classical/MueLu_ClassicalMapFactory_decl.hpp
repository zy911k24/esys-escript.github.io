// @HEADER
//
// ***********************************************************************
//
//        MueLu: A package for multigrid based preconditioning
//                  Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER

#ifndef MUELU_CLASSICALMAPFACTORY_DECL_HPP_
#define MUELU_CLASSICALMAPFACTORY_DECL_HPP_

#include "Xpetra_StridedMapFactory_fwd.hpp"
#include "Xpetra_Import_fwd.hpp"
#include "Xpetra_Vector_fwd.hpp"
#include "Xpetra_VectorFactory_fwd.hpp"

#include "MueLu_ConfigDefs.hpp"
#include "MueLu_SingleLevelFactoryBase.hpp"
#include "MueLu_ClassicalMapFactory_fwd.hpp"
#include "MueLu_GraphBase_fwd.hpp"
#include "MueLu_Level_fwd.hpp"
#include "MueLu_Exceptions.hpp"
#include "MueLu_Graph_fwd.hpp"
#include "MueLu_LWGraph_fwd.hpp"
#include "MueLu_LWGraph_kokkos_fwd.hpp"

namespace MueLu {

  /*!
    @class ClassicalMapFactory class.
    @brief Factory for generating F/C-splitting and a coarse level map. Used by ClassicalPFactory.

    @ingroup MueLuTransferClasses

    ## Input/output ##

    ### User parameters of this factory ###
    Parameter | type | default | master.xml | validated | requested | description
    ----------|------|---------|:----------:|:---------:|:---------:|------------
    Graph | Factory | null | | * | * | Generating factory for graph.
    The * in the @c master.xml column denotes that the parameter is defined in the @c master.xml file.<br>
    The * in the @c validated column means that the parameter is declared in the list of valid input parameters (see @c GetValidParameters() ).<br>
    The * in the @c requested column states that the data is requested as input with all dependencies (see @c DeclareInput() ).


    ### Variables provided by this factory ###

    After @c Build() the following data is available (if requested)

    Parameter | generated by | description
    ----------|--------------|------------
    | Colors | ClassicalMapFactory | ArrayRCP<LO> of colors
    | CoarseMap | CoarseMapFactory | Map containing the coarse map used as domain map in the classical prolongator

  */

  template <class Scalar = DefaultScalar,
          class LocalOrdinal = DefaultLocalOrdinal,
          class GlobalOrdinal = DefaultGlobalOrdinal,
          class Node = DefaultNode>
  class ClassicalMapFactory : public SingleLevelFactoryBase {
#undef MUELU_CLASSICALMAPFACTORY_SHORT
#include "MueLu_UseShortNames.hpp"

  public:
    //! F/C/Dirichlet point type
    typedef enum {F_PT=-1, UNASSIGNED=0, C_PT=1, DIRICHLET_PT=2} point_type;

    //! @name Input
    //@{

    RCP<const ParameterList> GetValidParameterList() const override;

    /*!
      @brief Specifies the data that this class needs, and the factories that generate that data.

      If the Build method of this class requires some data, but the generating factory is not specified in DeclareInput,
      then this class will fall back to the settings in FactoryManager.
    */
    void DeclareInput(Level &currentLevel) const override;

    //@}

    //! @name Build methods.
    //@{

    //! Build an object with this factory.
    void Build(Level &currentLevel) const override;

    //@}


  protected:
    virtual void GenerateCoarseMap(const Map & fineMap, LO num_c_points, Teuchos::RCP<const Map> & coarseMap) const;

    virtual void DoGraphColoring(const GraphBase & graph, Teuchos::ArrayRCP<LO> & myColors, LO & numColors) const;

    virtual void DoMISNaive(const GraphBase & graph, Teuchos::ArrayRCP<LO> & myColors, LO & numColors) const;

    virtual void DoDistributedGraphColoring(RCP<const GraphBase> & graph, Teuchos::ArrayRCP<LO> & myColors, LO & numColors) const;

  }; //class ClassicalMapFactory

} //namespace MueLu

#define MUELU_CLASSICALMAPFACTORY_SHORT
#endif /* MUELU_CLASSICALMAPFACTORY_DECL_HPP_ */