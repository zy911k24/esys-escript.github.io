
//@HEADER
// ************************************************************************
//
//            LOCA: Library of Continuation Algorithms Package
//                 Copyright (2005) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
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
// Questions? Contact Roger Pawlowski (rppawlo@sandia.gov) or
// Eric Phipps (etphipp@sandia.gov), Sandia National Laboratories.
// ************************************************************************
//  CVS Information
// ************************************************************************
//@HEADER

#ifndef LOCA_TURNINGPOINT_MOORESPENCE_SALINGERBORDERING_H
#define LOCA_TURNINGPOINT_MOORESPENCE_SALINGERBORDERING_H

#include "LOCA_TurningPoint_MooreSpence_SolverStrategy.H"
#include "NOX_Abstract_MultiVector.H"

// forward declarations
namespace LOCA {
  class GlobalData;
  namespace Parameter {
    class SublistParser;
  }
}

namespace LOCA {

  namespace TurningPoint {

    namespace MooreSpence {

      /*!
       * \brief Moore-Spence turning point solver strategy based on
       * "Salinger" bordering.  This is the classic 4-solve bordering
       * method.
       */
      /*!
       * This class solves the Moore-Spence turning point Newton equations:
       * \f[
       *     \begin{bmatrix}
       *          J   &    0   &   f_p \\
       *       (Jv)_x &    J   & (Jv)_p \\
       *          0   & \phi^T &   0
       *     \end{bmatrix}
       *     \begin{bmatrix}
       *        X \\
       *        Y \\
       *        z
       *     \end{bmatrix} =
       *     \begin{bmatrix}
       *        F \\
       *        G \\
       *        h
       *     \end{bmatrix}
       *  \f]
       *  via the following block elimination scheme:
       *  \f[
       *    \begin{split}
       *       J [A \; b] &= [F \; f_p] \\
       *       J [C \; d] &= (Jv)_x[A \; b] - [G \; (Jv)_p] \\
       *       z &= (h + \phi^T C) / \phi^T d \\
       *       X &= A - b z \\
       *       Y &= -C + d z
       *    \end{split}
       *   \f]
       */
      class SalingerBordering :
    public LOCA::TurningPoint::MooreSpence::SolverStrategy {

      public:

    //! Constructor.
    /*!
     * \param global_data [in] Global data object
     * \param topParams [in] Parsed top-level parameter list
     * \param solverParams [in] Bordered solver parameters.  Currently none
     * are referenced.
     */
    SalingerBordering(
     const Teuchos::RCP<LOCA::GlobalData>& global_data,
     const Teuchos::RCP<LOCA::Parameter::SublistParser>& topParams,
     const Teuchos::RCP<Teuchos::ParameterList>& solverParams);

    //! Destructor
    virtual ~SalingerBordering();

    //! Set blocks in extended linear system
    /*!
     * \param group [in] Underlying group representing J
     * \param tpGroup [in] Turning point group representing the turning
     *        point equations.
     * \param nullVector [in] Vector representing v
     * \param JnVector [in] Vector representing Jv
     * \param dfdp [in] Vector representing df/dp
     * \param dJndp [in] Vector representing d(Jv)/dp
     */
    virtual void setBlocks(
      const Teuchos::RCP<LOCA::TurningPoint::MooreSpence::AbstractGroup>& group,
      const Teuchos::RCP<LOCA::TurningPoint::MooreSpence::ExtendedGroup>& tpGroup,
      const Teuchos::RCP<const NOX::Abstract::Vector>& nullVector,
      const Teuchos::RCP<const NOX::Abstract::Vector>& JnVector,
      const Teuchos::RCP<const NOX::Abstract::MultiVector>& dfdp,
      const Teuchos::RCP<const NOX::Abstract::MultiVector>& dJndp);

    /*!
     * \brief Solves the extended system as defined above
     */
    /*!
     * The \em params argument is the linear solver parameters.
     */
    virtual NOX::Abstract::Group::ReturnType
    solve(
       Teuchos::ParameterList& params,
       const LOCA::TurningPoint::MooreSpence::ExtendedMultiVector& input,
       LOCA::TurningPoint::MooreSpence::ExtendedMultiVector& result) const;

    /*!
     * \brief Solves the transpose of the extended system as defined above
     */
    /*!
     * The \em params argument is the linear solver parameters.
     */
    virtual NOX::Abstract::Group::ReturnType
    solveTranspose(
      Teuchos::ParameterList& params,
      const LOCA::TurningPoint::MooreSpence::ExtendedMultiVector& input,
      LOCA::TurningPoint::MooreSpence::ExtendedMultiVector& result) const;

      protected:

    //! Solves equations with contiguous arguments
    NOX::Abstract::Group::ReturnType
    solveContiguous(
              Teuchos::ParameterList& params,
          const NOX::Abstract::MultiVector& input_x,
          const NOX::Abstract::MultiVector& input_null,
              const NOX::Abstract::MultiVector::DenseMatrix& input_param,
          NOX::Abstract::MultiVector& result_x,
          NOX::Abstract::MultiVector& result_null,
              NOX::Abstract::MultiVector::DenseMatrix& result_param) const;

    //! Solves equations with contiguous arguments
    NOX::Abstract::Group::ReturnType
    solveTransposeContiguous(
              Teuchos::ParameterList& params,
          const NOX::Abstract::MultiVector& input_x,
          const NOX::Abstract::MultiVector& input_null,
              const NOX::Abstract::MultiVector::DenseMatrix& input_param,
          NOX::Abstract::MultiVector& result_x,
          NOX::Abstract::MultiVector& result_null,
              NOX::Abstract::MultiVector::DenseMatrix& result_param) const;

      private:

    //! Private to prohibit copying
    SalingerBordering(const SalingerBordering&);

    //! Private to prohibit copying
    SalingerBordering& operator = (const SalingerBordering&);

      protected:

    //! Global data object
    Teuchos::RCP<LOCA::GlobalData> globalData;

    //! Solver parameters
    Teuchos::RCP<Teuchos::ParameterList> solverParams;

    //! Underlying group
    Teuchos::RCP<LOCA::TurningPoint::MooreSpence::AbstractGroup> group;

    //! Turning point group
    Teuchos::RCP<LOCA::TurningPoint::MooreSpence::ExtendedGroup> tpGroup;

    //! Null vector
    Teuchos::RCP<const NOX::Abstract::Vector> nullVector;

    //! Jacobian times null vector
    Teuchos::RCP<const NOX::Abstract::Vector> JnVector;

    //! df/dp
    Teuchos::RCP<const NOX::Abstract::MultiVector> dfdp;

    //! d(Jn)/dp
    Teuchos::RCP<const NOX::Abstract::MultiVector> dJndp;

      }; // Class SalingerBordering

    } // Namespace MooreSpence

  } // Namespace TurningPoint

} // Namespace LOCA

#endif // LOCA_TURNINGPOINT_MOORESPENCE_SALINGERBORDERING_H
