//@HEADER
// ***********************************************************************
//
//     EpetraExt: Epetra Extended - Linear Algebra Services Package
//                 Copyright (2011) Sandia Corporation
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
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ***********************************************************************
//@HEADER
                                                                                                    
#ifndef EpetraExt_LINEARPROBLEM_GRAPHTRANS_H
#define EpetraExt_LINEARPROBLEM_GRAPHTRANS_H

#include <EpetraExt_Transform.h>

class Epetra_LinearProblem;
class Epetra_Map;
class Epetra_MultiVector;
class Epetra_CrsGraph;
class Epetra_CrsMatrix;
class Epetra_Export;
class Epetra_Import;

namespace EpetraExt {

//! EpetraExt::LinearProblem_GraphTrans: Adaptation of a Epetra_CrsGraph Transform to a Epetra_LinearProblem Transform
class LinearProblem_GraphTrans : public SameTypeTransform<Epetra_LinearProblem>
{
  StructuralSameTypeTransform<Epetra_CrsGraph> & graphTrans_;

  Epetra_Import * Importer_;
  Epetra_Export * MatExporter_;
  Epetra_Export * VecExporter_;

  Epetra_LinearProblem * OldProblem_;
  Epetra_CrsGraph * OldGraph_;
  Epetra_CrsMatrix * OldMatrix_;
  Epetra_MultiVector * OldLHS_;
  Epetra_MultiVector * OldRHS_;
  Epetra_Map * OldRowMap_;

  Epetra_LinearProblem * NewProblem_;
  Epetra_CrsMatrix * NewMatrix_;
  Epetra_MultiVector * NewLHS_;
  Epetra_MultiVector * NewRHS_;

 public:

  //! EpetraExt::LinearProblem_GraphTrans Destructor
  ~LinearProblem_GraphTrans();

  //! EpetraExt::LinearProblem_GraphTrans Constructor
  /*! Constructs a LinearProblem Transform based on the input CrsGraph Transform
      \param In
      graph_trans - Base Epetra_CrsGraph Transform from which a consistent Epetra_LinearProblem  Transform is generated
 */
  LinearProblem_GraphTrans( StructuralSameTypeTransform<Epetra_CrsGraph> & graph_trans )
  : graphTrans_(graph_trans),
    Importer_(0),
    MatExporter_(0),
    VecExporter_(0),
    OldProblem_(0),
    OldGraph_(0),
    OldMatrix_(0),
    OldLHS_(0),
    OldRHS_(0),
    OldRowMap_(0),
    NewProblem_(0),
    NewMatrix_(0),
    NewLHS_(0),
    NewRHS_(0)
  {}

  //! Constructs an Epetra_LinearProblem from the original using the same row transformation given by the Epetra_CrsGraph Transform
  /*! 
      \param In
      orig - Original Epetra_LinearProblem to be transformed.
      \return Epetra_LinearProblem generated by transformation operation
  */
  NewTypeRef operator()( OriginalTypeRef orig );

  //! Forward migration of data from original to transformed object
  bool fwd();

  //! Reverse migration of data from transformed to original object
  bool rvs();

};

} //namespace EpetraExt

#endif //EpetraExt_LINEARPROBLEM_GRAPHTRANS_H


#if defined(EpetraExt_SHOW_DEPRECATED_WARNINGS)
#ifdef __GNUC__
#warning "The EpetraExt package is deprecated"
#endif
#endif
