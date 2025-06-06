// Copyright(C) 1999-2020, 2023 National Technology & Engineering Solutions
// of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
// NTESS, the U.S. Government retains certain rights in this software.
//
// See packages/seacas/LICENSE for details

#include <Ioss_DatabaseIO.h>
#include <Ioss_EdgeSet.h>
#include <Ioss_Field.h>
#include <Ioss_Property.h>
#include <cstddef>
#include <string>
#include <vector>

#include "Ioss_EntitySet.h"
#include "Ioss_FieldManager.h"
#include "Ioss_GroupingEntity.h"

Ioss::EdgeSet::EdgeSet() : Ioss::EntitySet(nullptr, "invalid", 0) {}

/** \brief Create an edge set.
 *
 *  \param[in] io_database The database associated with the region containing the edge set.
 *  \param[in] my_name The edge set's name.
 *  \param[in] number_edges The number of edges in the edge set.
 */
Ioss::EdgeSet::EdgeSet(Ioss::DatabaseIO *io_database, const std::string &my_name,
                       int64_t number_edges)
    : Ioss::EntitySet(io_database, my_name, number_edges)
{
  // Add the standard fields...
  fields.add(
      Ioss::Field("orientation", Ioss::Field::INTEGER, "scalar", Ioss::Field::MESH, number_edges));
}

int64_t Ioss::EdgeSet::internal_get_field_data(const Ioss::Field &field, void *data,
                                               size_t data_size) const
{
  return get_database()->get_field(this, field, data, data_size);
}

int64_t Ioss::EdgeSet::internal_put_field_data(const Ioss::Field &field, void *data,
                                               size_t data_size) const
{
  return get_database()->put_field(this, field, data, data_size);
}

int64_t Ioss::EdgeSet::internal_get_zc_field_data(const Field &field, void **data,
                                                  size_t *data_size) const
{
  return get_database()->get_zc_field(this, field, data, data_size);
}

Ioss::Property Ioss::EdgeSet::get_implicit_property(const std::string &my_name) const
{
  return Ioss::EntitySet::get_implicit_property(my_name);
}

void Ioss::EdgeSet::block_membership(std::vector<std::string> & /*block_members*/) {}
