// @HEADER
// ***********************************************************************
//
//                           Sacado Package
//                 Copyright (2006) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
// USA
// Questions? Contact David M. Gay (dmgay@sandia.gov) or Eric T. Phipps
// (etphipp@sandia.gov).
//
// ***********************************************************************
// @HEADER

#include "Sacado_ConfigDefs.h"

#ifdef HAVE_SACADO_COMPLEX

#include "FadUnitTests2.hpp"

typedef ::testing::Types<
  Sacado::ELRCacheFad::DFad<std::complex<double>>,
  Sacado::ELRCacheFad::SFad<std::complex<double>,5>,
  Sacado::ELRCacheFad::SLFad<std::complex<double>,5>
  > FadTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(ComplexELRCacheFad, FadOpsUnitTest2, FadTypes);

#endif