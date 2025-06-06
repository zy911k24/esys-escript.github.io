
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

#ifndef LOCA_PITCHFORK_MOORESPENCE_PHIPPSBORDERING_H
#define LOCA_PITCHFORK_MOORESPENCE_PHIPPSBORDERING_H

#include "LOCA_Pitchfork_MooreSpence_SolverStrategy.H"
#include "NOX_Abstract_MultiVector.H"

// forward declarations
namespace LOCA {
  class GlobalData;
  namespace Parameter {
    class SublistParser;
  }
  namespace BorderedSolver {
    class AbstractStrategy;
  }
}

namespace LOCA {

  namespace Pitchfork {

    namespace MooreSpence {

      /*!
       * \brief Moore-Spence pitchfork solver strategy based on
       * "Phipps" bordering which is the 7-solve modified pitchfork
       * bordering algorithm that uses bordered linear solves.
       */
      /*!
       * This class solves the Moore-Spence pitchfork Newton equations:
       * \f[
       *     \begin{bmatrix}
       *          J   &    0   & \psi &  f_p   \\
       *       (Jv)_x &    J   &   0  & (Jv)_p \\
       *       \psi^T &    0   &   0  &   0    \\
       *          0   & \phi^T &   0  &   0
       *     \end{bmatrix}
       *     \begin{bmatrix}
       *        X \\
       *        Y \\
       *        w \\
       *        z
       *     \end{bmatrix} =
       *     \begin{bmatrix}
       *        F \\
       *        G \\
       *        s \\
       *        h
       *     \end{bmatrix}
       *  \f]
       *  via the following modified block elimination scheme:
       *  \f[
       *    \begin{split}
       *      \begin{bmatrix}
       *         J   & u \\
       *         v^T & 0
       *      \end{bmatrix}
       *      \begin{bmatrix}
       *         A & B & C \\
       *         a & b & c
       *      \end{bmatrix} &=
       *      \begin{bmatrix}
       *         F & f_p & \psi \\
       *         0 & 0   &  0
       *      \end{bmatrix} \\
       *      \begin{bmatrix}
       *         J   & u \\
       *         v^T & 0
       *      \end{bmatrix}
       *      \begin{bmatrix}
       *         D & E & K & L \\
       *         d & e & k & l
       *      \end{bmatrix} &=
       *      \begin{bmatrix}
       *         G - (Jv)_x A & (Jv)_p - (Jv)_x B &  -(Jv)_x C & -(Jv)_x v \\
       *                0     &          0        &       0    &      0
       *      \end{bmatrix} \\
       *      \begin{bmatrix}
       *         \sigma & 0 & b & c \\
       *          -l & \sigma & e & k \\
       *          \langle v,\psi\rangle & 0 & -\langle B,\psi\rangle & -\langle C,\psi\rangle \\
       *         \phi^T L & \phi^T v & -\phi^T E & -\phi^T K
       *      \end{bmatrix}
       *      \begin{bmatrix}
       *         \alpha \\
       *         \beta \\
       *         z \\
       *         w
       *      \end{bmatrix} &=
       *      \begin{bmatrix}
       *         a \\
       *         d \\
       *         s - \langle A,\psi\rangle
       *         h - \phi^T D
       *      \end{bmatrix} \\
       *       X &= A - B z - C w + v \alpha \\
       *       Y &= D - E z - K w + L \alpha + v \beta
       *    \end{split}
       *   \f]
       * where \f$\sigma = \|J v\|\f$ and \f$u = J v/\sigma\f$.  Each bordered
       * solve is implemented by a LOCA::BorderedSolver::AbstractStrategy
       * strategy object.
       */
      class PhippsBordering :
    public LOCA::Pitchfork::MooreSpence::SolverStrategy {

      public:

    //! Constructor.
    /*!
     * \param global_data [in] Global data object
     * \param topParams [in] Parsed top-level parameter list
     * \param solverParams [in] Bordered solver parameters.  Instantiates
     *        a bordered solver for solving the bordeded systems
     *        described above.  See LOCA::BorderedSolver::Factory for a
     *        description of available solvers.
     */
    PhippsBordering(
     const Teuchos::RCP<LOCA::GlobalData>& global_data,
     const Teuchos::RCP<LOCA::Parameter::SublistParser>& topParams,
     const Teuchos::RCP<Teuchos::ParameterList>& solverParams);

    //! Destructor
    virtual ~PhippsBordering();

    //! Set blocks in extended linear system
    /*!
     * \param group [in] Underlying group representing J
     * \param pfGroup [in] Pitchfork group representing the pitchfork
     *                     equations.
     * \param asymMultiVector [in] Multivector representing the
     *                             asymmetric vector
     * \param nullVector [in] Vector representing v
     * \param JnVector [in] Vector representing Jv
     * \param dfdp [in] Vector representing df/dp
     * \param dJndp [in] Vector representing d(Jv)/dp
     */
    virtual void setBlocks(
      const Teuchos::RCP<LOCA::Pitchfork::MooreSpence::AbstractGroup>& group,
      const Teuchos::RCP<LOCA::Pitchfork::MooreSpence::ExtendedGroup>& pfGroup,
      const Teuchos::RCP<const NOX::Abstract::MultiVector>& asymMultiVector,
      const Teuchos::RCP<const NOX::Abstract::Vector>& nullVector,
      const Teuchos::RCP<const NOX::Abstract::Vector>& JnVector,
      const Teuchos::RCP<const NOX::Abstract::Vector>& dfdp,
      const Teuchos::RCP<const NOX::Abstract::Vector>& dJndp);

    /*!
     * \brief Solves the extended system as defined above
     */
    /*!
     * The \em params argument is the linear solver parameters.
     */
    virtual NOX::Abstract::Group::ReturnType
    solve(
         Teuchos::ParameterList& params,
             const LOCA::Pitchfork::MooreSpence::ExtendedMultiVector& input,
             LOCA::Pitchfork::MooreSpence::ExtendedMultiVector& result) const;

      protected:

    //! Solves equations with contiguous arguments
    NOX::Abstract::Group::ReturnType
    solveContiguous(
              Teuchos::ParameterList& params,
          const NOX::Abstract::MultiVector& input_x,
          const NOX::Abstract::MultiVector& input_null,
          const NOX::Abstract::MultiVector::DenseMatrix& input_slack,
              const NOX::Abstract::MultiVector::DenseMatrix& input_param,
          NOX::Abstract::MultiVector& result_x,
          NOX::Abstract::MultiVector& result_null,
          NOX::Abstract::MultiVector::DenseMatrix& result_slack,
              NOX::Abstract::MultiVector::DenseMatrix& result_param) const;

      private:

    //! Private to prohibit copying
    PhippsBordering(const PhippsBordering&);

    //! Private to prohibit copying
    PhippsBordering& operator = (const PhippsBordering&);

      protected:

    //! Global data object
    Teuchos::RCP<LOCA::GlobalData> globalData;

    //! Solver parameters
    Teuchos::RCP<Teuchos::ParameterList> solverParams;

    //! Underlying group
    Teuchos::RCP<LOCA::Pitchfork::MooreSpence::AbstractGroup> group;

    //! Turning point group
    Teuchos::RCP<LOCA::Pitchfork::MooreSpence::ExtendedGroup> pfGroup;

    //! Antisymmetric vector (psi)
    Teuchos::RCP<const NOX::Abstract::MultiVector> asymMultiVector;

    //! Antisymmetric vector (psi)
    Teuchos::RCP<const NOX::Abstract::Vector> asymVector;

    //! Null vector
    Teuchos::RCP<const NOX::Abstract::Vector> nullVector;

    //! Jacobian times null vector
    Teuchos::RCP<const NOX::Abstract::Vector> JnVector;

    //! df/dp
    Teuchos::RCP<const NOX::Abstract::Vector> dfdp;

    //! d(Jn)/dp
    Teuchos::RCP<const NOX::Abstract::Vector> dJndp;

    //! Bordered solver for solving (n+1)x(n+1) sets of equations
    Teuchos::RCP<LOCA::BorderedSolver::AbstractStrategy> borderedSolver;

    //! Null vector as a multivector
    Teuchos::RCP<NOX::Abstract::MultiVector> nullMultiVector;

    //! Jacobian times null vector as a multivector
    Teuchos::RCP<NOX::Abstract::MultiVector> JnMultiVector;

    //! Norm of JnVector
    double sigma;

      }; // Class PhippsBordering

    } // Namespace MooreSpence

  } // Namespace Pitchfork

} // Namespace LOCA

#endif // LOCA_PITCHFORK_MOORESPENCE_PHIPPSBORDERING_H
