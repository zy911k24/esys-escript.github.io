
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

#ifndef LOCA_BORDEREDSOLVER_HOUSEHOLDERQR_H
#define LOCA_BORDEREDSOLVER_HOUSEHOLDERQR_H

#include "Teuchos_RCP.hpp"  // class data element
#include "Teuchos_BLAS.hpp"         // class data element

#include "NOX_Abstract_MultiVector.H" // for DenseMatrix

// forward declarations
namespace LOCA {
  class GlobalData;
}

namespace LOCA {

  namespace BorderedSolver {

    /*!
     * \brief A convenience class to compute the QR factorization of a an
     * extended multi-vector.
     */
    /*!
     * This class computes the QR factorization
     * \f[
     *     Q^T
     *     \begin{bmatrix}
     *        op(C) \\
     *        B
     *     \end{bmatrix} =
     *     \begin{bmatrix}
     *        R \\
     *        0
     *     \end{bmatrix}
     * \f]
     * where \f$C\f$ is an \f$m\times m\f$ matrix, \f$B\f$ is an
     * \f$n\times m\f$ matrix, \f$Q\f$ is an \f$n+m\times n+m\f$ matrix,
     * \f$R\f$ is an \f$m\times m\f$ matrix, and \f$op()\f$ represents either
     * the identity operation or the transpose.  The matrix \f$C\f$ is
     * represented by a NOX::Abstract::MultiVector::DenseMatrix while
     * \f$B\f$ is a NOX::Abstract::MultiVector.  Given \f$B\f$ and \f$C\f$,
     * this class computes \f$Q\f$ and \f$R\f$ with \f$R\f$ returned as
     * NOX::Abstract::MultiVector::DenseMatrix.  The
     * operator \f$Q\f$ is generated using the standard Householder QR
     * algorithm (Algorithm 5.2.1, G. Golub and C. Van Loan, "Matrix
     * Computations," 3rd Edition, Johns Hopkins, Baltimore, 1996) and is
     * stored using the compact WY representation:  \f$Q = I + Y^T T Y\f$
     * (see R. Schreiver and C. Van Loan, "A Storage-Efficient WY Represntation
     * for Products of Householder Transformations," SIAM J. Sci. Stat.
     * Comput., Vol. 10, No. 1, pp. 53-57, January 1989).
     */
    class HouseholderQR {

    public:

      //! Constructor.
      /*!
       * \param global_data [in] Global data object
       */
      HouseholderQR(const Teuchos::RCP<LOCA::GlobalData>& global_data);

      //! Destructor
      virtual ~HouseholderQR();

      //! Compute QR factorization as described above
      /*!
       * Set \c use_c_transpose to true if the transpose of \f$C\f$ is
       * required.
       */
      void computeQR(const NOX::Abstract::MultiVector::DenseMatrix& C,
             const NOX::Abstract::MultiVector& B,
             bool use_c_transpose,
             NOX::Abstract::MultiVector::DenseMatrix& Y1,
             NOX::Abstract::MultiVector& Y2,
             NOX::Abstract::MultiVector::DenseMatrix& T,
             NOX::Abstract::MultiVector::DenseMatrix& R);

      /*!
       * \brief Applies the operator Q as described above overwriting
       * \c x and \c y.  If either of \c x or \c y are zero on input,
       * set the corresponding \c isZeroX or \c isZeroY flags.  Set\
       * \c useTranspose to \c true to instead apply the transpose of Q.
       */
      void applyCompactWY(const NOX::Abstract::MultiVector::DenseMatrix& Y1,
              const NOX::Abstract::MultiVector& Y2,
              const NOX::Abstract::MultiVector::DenseMatrix& T,
              NOX::Abstract::MultiVector::DenseMatrix& X1,
              NOX::Abstract::MultiVector& X2,
              bool isZeroX1, bool isZeroX2,
              bool useTranspose) const;

      /*!
       * \brief Another version of applyCompactWY() that does not overwrite
       * its inputs.  If either input is zero, set the corresponding pointer
       * to NULL.
       */
      void applyCompactWY(
               const NOX::Abstract::MultiVector::DenseMatrix& Y1,
               const NOX::Abstract::MultiVector& Y2,
               const NOX::Abstract::MultiVector::DenseMatrix& T,
               const NOX::Abstract::MultiVector::DenseMatrix* input1,
               const NOX::Abstract::MultiVector* input2,
               NOX::Abstract::MultiVector::DenseMatrix& result1,
               NOX::Abstract::MultiVector& result2,
               bool useTranspose) const;

    protected:

      /*!
       * \brief Computes the Householder vector \c V1, \c V2 for column
       * \c col of \c A1 and \c A2, starting at row \c col of A1.
       */
      /*!
       * The algorithm implemented here is essentially algorithm 5.1.1
       * of Golub and Van Loan.
       */
      void computeHouseholderVector(
              int col,
              const NOX::Abstract::MultiVector::DenseMatrix& A1,
              const NOX::Abstract::MultiVector& A2,
              NOX::Abstract::MultiVector::DenseMatrix& V1,
              NOX::Abstract::MultiVector& V2,
              double& beta);

      /*!
       * \brief Applies the Householder vector \c V1, \c V2 to the
       * matrix sub-block represented by \c A1 and A2.
       */
      void applyHouseholderVector(
               const NOX::Abstract::MultiVector::DenseMatrix& V1,
               const NOX::Abstract::MultiVector& V2,
               double beta,
               NOX::Abstract::MultiVector::DenseMatrix& A1,
               NOX::Abstract::MultiVector& A2);

    private:

      //! Private to prohibit copying
      HouseholderQR(const HouseholderQR&);

      //! Private to prohibit copying
      HouseholderQR& operator = (const HouseholderQR&);

    protected:

      //! Global data object
      Teuchos::RCP<LOCA::GlobalData> globalData;

      //! BLAS Wrappers
      Teuchos::BLAS<int,double> dblas;

    };
  } // namespace BorderedSolver
} // namespace LOCA

#endif
