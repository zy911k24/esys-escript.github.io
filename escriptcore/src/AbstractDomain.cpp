
/*****************************************************************************
*
* Copyright (c) 2003-2020 by The University of Queensland
* http://www.uq.edu.au
*
* Primary Business: Queensland, Australia
* Licensed under the Apache License, version 2.0
* http://www.apache.org/licenses/LICENSE-2.0
*
* Development until 2012 by Earth Systems Science Computational Center (ESSCC)
* Development 2012-2013 by School of Earth Sciences
* Development from 2014-2017 by Centre for Geoscience Computing (GeoComp)
* Development from 2019 by School of Earth and Environmental Sciences
**
*****************************************************************************/

#include "AbstractDomain.h" 
#include "DomainException.h"

namespace escript {

// Please see the discussion in DataAbstract's version of this method
// and why I squash the exception
Domain_ptr AbstractDomain::getPtr()
{
    try {
        return shared_from_this();
    } catch (boost::bad_weak_ptr&) {
        return Domain_ptr(this);
    }
}

const_Domain_ptr AbstractDomain::getPtr() const 
{  
    try {
        return shared_from_this();
    } catch (boost::bad_weak_ptr&) {
        return const_Domain_ptr(this);
    }
}

void AbstractDomain::throwStandardException(const std::string& functionName) const
{
  throw DomainException("Error - Base class function: " + functionName + " should not be called. Programming error.");
}

AbstractDomain::StatusType AbstractDomain::getStatus() const 
{
    return 0;
}

bool AbstractDomain::isValidTagName(const std::string& name) const
{
    return false;
}

bool AbstractDomain::supportsFilter(const boost::python::tuple& t) const
{
    if (len(t)==0) {
        return true; // to make creating non-filtered values simpler 
    }
    return false;  
}

#if defined(ESYS_MPI) && defined(ESYS_HAVE_MPI4PY)
// void AbstractDomain::setMPIComm(boost::python::object py_comm)
// {
//     PyObject* py_obj = py_comm.ptr();
//     MPI_Comm *comm_p = PyMPIComm_Get(py_obj);
//     if (comm_p == NULL) 
//         throw EsysException("Invalid MPI communicator.");

//     JMPI old_comm = getMPI();
//     old_comm->comm = *comm_p;
// }
#endif

} // end of namespace

