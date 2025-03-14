// @HEADER
// *****************************************************************************
//           Amesos2: Templated Direct Sparse Solver Package
//
// Copyright 2011 NTESS and the Amesos2 contributors.
// SPDX-License-Identifier: BSD-3-Clause
// *****************************************************************************
// @HEADER

#include <Teuchos_ScalarTraits.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_oblackholestream.hpp>
#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_CommandLineProcessor.hpp>

#include <Tpetra_Core.hpp>
#include <Tpetra_Map.hpp>
#include <Tpetra_MultiVector.hpp>
#include <Tpetra_CrsMatrix.hpp>

// I/O for Matrix-Market files
#include <MatrixMarket_Tpetra.hpp>
#include <Tpetra_Import.hpp>

#include "Amesos2.hpp"
#include "Amesos2_Version.hpp"


int main(int argc, char *argv[]) {
  Tpetra::ScopeGuard tpetraScope(&argc,&argv);

  typedef Tpetra::CrsMatrix<>::scalar_type Scalar;
  typedef Tpetra::Map<>::local_ordinal_type LO;
  typedef Tpetra::Map<>::global_ordinal_type GO;

  typedef Tpetra::CrsMatrix<Scalar,LO,GO> MAT;
  typedef Tpetra::MultiVector<Scalar,LO,GO> MV;

  using Tpetra::global_size_t;
  using Tpetra::Map;
  using Tpetra::Import;
  using Teuchos::RCP;
  using Teuchos::rcp;


  //
  // Get the default communicator
  //
  Teuchos::RCP<const Teuchos::Comm<int> > comm = Tpetra::getDefaultComm();
  int myRank = comm->getRank();

  Teuchos::oblackholestream blackhole;

  bool printMatrix   = false;
  bool printSolution = false;
  bool printTiming   = false;
  bool allprint      = false;
  bool verbose = (myRank==0);
  std::string filename("arc130.mtx");
  Teuchos::CommandLineProcessor cmdp(false,true);
  cmdp.setOption("verbose","quiet",&verbose,"Print messages and results.");
  cmdp.setOption("filename",&filename,"Filename for Matrix-Market test matrix.");
  cmdp.setOption("print-matrix","no-print-matrix",&printMatrix,"Print the full matrix after reading it.");
  cmdp.setOption("print-solution","no-print-solution",&printSolution,"Print solution vector after solve.");
  cmdp.setOption("print-timing","no-print-timing",&printTiming,"Print solver timing statistics");
  cmdp.setOption("all-print","root-print",&allprint,"All processors print to out");
  if (cmdp.parse(argc,argv) != Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL) {
    return -1;
  }

  std::ostream& out = ( (allprint || (myRank == 0)) ? std::cout : blackhole );
  RCP<Teuchos::FancyOStream> fos = Teuchos::fancyOStream(Teuchos::rcpFromRef(out));

  // Say hello
  out << myRank << " : " << Amesos2::version() << std::endl << std::endl;

  const size_t numVectors = 1;

  RCP<MAT> A = Tpetra::MatrixMarket::Reader<MAT>::readSparseFile(filename, comm);
  if( printMatrix ){
    A->describe(*fos, Teuchos::VERB_EXTREME);
  }
  else if( verbose ){
    std::cout << std::endl << A->description() << std::endl << std::endl;
  }

  // get the maps
  RCP<const Map<LO,GO> > dmnmap = A->getDomainMap();
  RCP<const Map<LO,GO> > rngmap = A->getRangeMap();

  GO nrows = dmnmap->getGlobalNumElements();
  RCP<Map<LO,GO> > root_map
    = rcp( new Map<LO,GO>(nrows,myRank == 0 ? nrows : 0,0,comm) );
  RCP<MV> Xhat = rcp( new MV(root_map,numVectors) );
  RCP<Import<LO,GO> > importer = rcp( new Import<LO,GO>(dmnmap,root_map) );

  // Create random X
  RCP<MV> X = rcp(new MV(dmnmap,numVectors));
  X->randomize();

  /* Create B
   *
   * Use RHS:
   *
   *  [[10]
   *   [10]
   *   [10]
   *   [10]
   *   [10]
   *   [10]]
   */
  RCP<MV> B = rcp(new MV(rngmap,numVectors));
  B->putScalar(10);


  // Constructor from Factory
  RCP<Amesos2::Solver<MAT,MV> > solver;
  if( !Amesos2::query("Superlu") ){
    *fos << "SuperLU solver not enabled.  Exiting..." << std::endl;
    return EXIT_SUCCESS;
  }

  solver = Amesos2::create<MAT,MV>("Superlu", A, X, B);

  solver->symbolicFactorization().numericFactorization().solve();

  if( printSolution ){
    // Print the solution
    if( allprint ){
      if( myRank == 0 ) *fos << "Solution :" << std::endl;
      Xhat->describe(*fos,Teuchos::VERB_EXTREME);
      *fos << std::endl;
    } else {
      Xhat->doImport(*X,*importer,Tpetra::REPLACE);
      if( myRank == 0 ){
        *fos << "Solution :" << std::endl;
        Xhat->describe(*fos,Teuchos::VERB_EXTREME);
        *fos << std::endl;
      }
    }
  }

  if( printTiming ){
    // Print some timing statistics
    solver->printTiming(*fos);
  }
  Teuchos::TimeMonitor::summarize();

  // We are done.
  return 0;
}
