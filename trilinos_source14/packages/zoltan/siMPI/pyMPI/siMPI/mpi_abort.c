/*****************************************************************************
 * CVS File Information :
 *    $RCSfile$
 *    Author: patmiller $
 *    Date: 2007/06/11 14:12:54 $
 *    Revision: 1.2 $
 ****************************************************************************/
/***********************************************************************************************/
/* FILE  *************************        mpi_abort.c        ***********************************/
/***********************************************************************************************/
/* Author : Lisa Alano July 8 2002                                                             */
/* Copyright (c) 2002 University of California Regents                                         */
/***********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

/*==========================================================================*/
int mpi_abort(MPI_Comm* comm, int* errorcode )
{
  _MPI_COVERAGE();
  return PMPI_Abort(*comm, *errorcode);
}
/*==========================================================================*/