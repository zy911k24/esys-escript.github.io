
/*****************************************************************************
*
* Copyright (c) 2017-2018 by The University of Queensland
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

#ifndef __ESCRIPT_NCHELPER_H__
#define __ESCRIPT_NCHELPER_H__

#include "system_dep.h"

namespace escript
{
#ifndef _WIN32
#include <string>
#endif
ESCRIPT_DLL_API
char NcFType(const std::string& name);
}

#ifdef NETCDF4
#include <ncFile.h>
namespace escript
{
ESCRIPT_DLL_API
bool openNcFile(netCDF::NcFile& f, const std::string& name);
}
#endif

#endif
