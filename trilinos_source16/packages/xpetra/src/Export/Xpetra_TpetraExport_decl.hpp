// @HEADER
//
// ***********************************************************************
//
//             Xpetra: A linear algebra interface package
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
#ifndef XPETRA_TPETRAEXPORT_DECL_HPP
#define XPETRA_TPETRAEXPORT_DECL_HPP

/* this file is automatically generated - do not edit (see script/tpetra.py) */

#include "Xpetra_TpetraConfigDefs.hpp"

#include "Xpetra_Export.hpp"
#include "Xpetra_Exceptions.hpp"

#include "Xpetra_TpetraMap_decl.hpp"
#include "Tpetra_Export.hpp"

// Note: 'export' is a reserved keyword in C++. Do not use 'export' as a variable name.

namespace Xpetra {

// TODO: move that elsewhere
template <class LocalOrdinal, class GlobalOrdinal, class Node>
const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>& toTpetra(const Export<LocalOrdinal, GlobalOrdinal, Node>&);

template <class LocalOrdinal, class GlobalOrdinal, class Node>
RCP<const Export<LocalOrdinal, GlobalOrdinal, Node>>
toXpetra(const RCP<const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>>& exp);

template <class LocalOrdinal,
          class GlobalOrdinal,
          class Node>
class TpetraExport
  : public Export<LocalOrdinal, GlobalOrdinal, Node> {
 public:
  //! The specialization of Map used by this class.
  typedef Map<LocalOrdinal, GlobalOrdinal, Node> map_type;

  //! @name Constructor/Destructor Methods
  //@{

  //! Construct a Export object from the source and target Map.
  TpetraExport(const Teuchos::RCP<const map_type>& source, const Teuchos::RCP<const map_type>& target);

  //! Constructor (with list of parameters).
  TpetraExport(const Teuchos::RCP<const map_type>& source,
               const Teuchos::RCP<const map_type>& target,
               const Teuchos::RCP<Teuchos::ParameterList>& plist);

  //! Copy constructor.
  TpetraExport(const Export<LocalOrdinal, GlobalOrdinal, Node>& rhs);

  //! Destructor.
  ~TpetraExport();

  //@}

  //! @name Export Attribute Methods
  //@{

  //! Number of initial identical IDs.
  size_t getNumSameIDs() const;

  //! Number of IDs to permute but not to communicate.
  size_t getNumPermuteIDs() const;

  //! List of local IDs in the source Map that are permuted.
  ArrayView<const LocalOrdinal> getPermuteFromLIDs() const;

  //! List of local IDs in the target Map that are permuted.
  ArrayView<const LocalOrdinal> getPermuteToLIDs() const;

  //! Number of entries not on the calling process.
  size_t getNumRemoteIDs() const;

  //! List of entries in the target Map to receive from other processes.
  ArrayView<const LocalOrdinal> getRemoteLIDs() const;

  //! Number of entries that must be sent by the calling process to other processes.
  size_t getNumExportIDs() const;

  //! List of entries in the source Map that will be sent to other processes.
  ArrayView<const LocalOrdinal> getExportLIDs() const;

  //! List of processes to which entries will be sent.
  ArrayView<const int> getExportPIDs() const;

  //! The source Map used to construct this Export.
  Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node>> getSourceMap() const;

  //! The target Map used to construct this Export.
  Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node>> getTargetMap() const;

  //! Set distributor parameters.
  void setDistributorParameters(const Teuchos::RCP<Teuchos::ParameterList> params) const;

  //@}

  //! @name I/O Methods
  //@{

  //! Print the Export's data to the given output stream.
  void print(std::ostream& os) const;

  //@}

  //! @name Xpetra specific
  //@{

  //! TpetraExport constructor to wrap a Tpetra::Export object
  TpetraExport(const RCP<const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>>& exp);

  RCP<const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>> getTpetra_Export() const;

  //@}

 private:
  RCP<const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>> export_;

};  // TpetraExport class

// --------------
// Free functions
// --------------

// TODO: move that elsewhere
template <class LocalOrdinal, class GlobalOrdinal, class Node>
const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>&
toTpetra(const Export<LocalOrdinal, GlobalOrdinal, Node>& exp) {
  // TODO: throw exception
  const TpetraExport<LocalOrdinal, GlobalOrdinal, Node>& tpetraExport =
      dynamic_cast<const TpetraExport<LocalOrdinal, GlobalOrdinal, Node>&>(exp);
  return *tpetraExport.getTpetra_Export();
}

template <class LocalOrdinal, class GlobalOrdinal, class Node>
const RCP<const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>> toTpetra(const RCP<const Export<LocalOrdinal, GlobalOrdinal, Node>>& exportObj) {
  typedef TpetraExport<LocalOrdinal, GlobalOrdinal, Node> TpetraExportClass;
  if (exportObj != Teuchos::null) {
    XPETRA_RCP_DYNAMIC_CAST(const TpetraExportClass, rcpFromRef(*exportObj), tpetraExport, "toTpetra");
    return tpetraExport->getTpetra_Export();
  }
  return Teuchos::null;
}

template <class LocalOrdinal, class GlobalOrdinal, class Node>
RCP<const Export<LocalOrdinal, GlobalOrdinal, Node>>
toXpetra(const RCP<const Tpetra::Export<LocalOrdinal, GlobalOrdinal, Node>>& exp) {
  if (!exp.is_null())
    return rcp(new TpetraExport<LocalOrdinal, GlobalOrdinal, Node>(exp));

  return Teuchos::null;
}

}  // namespace Xpetra

#endif  // XPETRA_TPETRAEXPORT_DECL_HPP