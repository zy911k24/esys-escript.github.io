// Copyright(C) 1999-2021 National Technology & Engineering Solutions
// of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
// NTESS, the U.S. Government retains certain rights in this software.
//
// See packages/seacas/LICENSE for details

//------------------------------------------------------------------------
// Define a variable type for storage of this elements connectivity
#include "Ioss_CodeTypes.h"           // for IntVector
#include "Ioss_ElementTopology.h"     // for ElementTopology
#include <Ioss_ElementVariableType.h> // for ElementVariableType
#include <Ioss_Quad9.h>
#include <cassert> // for assert
#include <cstddef> // for nullptr

namespace Ioss {
  const char *Quad9::name = "quad9";
  class St_Quad9 : public ElementVariableType
  {
  public:
    static void factory() { static St_Quad9 registerThis; }

  protected:
    St_Quad9() : ElementVariableType(Ioss::Quad9::name, 9) {}
  };
} // namespace Ioss
// ========================================================================

namespace {
  struct Constants
  {
    static const int nnode     = 9;
    static const int nedge     = 4;
    static const int nedgenode = 3;
    static const int nface     = 0;
    static int       edge_node_order[nedge][nedgenode];
  };

  // Edge numbers are zero-based [0..number_edges)
  int Constants::edge_node_order[nedge][nedgenode] = // [edge][edge_node]
      {{0, 1, 4}, {1, 2, 5}, {2, 3, 6}, {3, 0, 7}};
} // namespace

void Ioss::Quad9::factory()
{
  static Ioss::Quad9 registerThis;
  Ioss::St_Quad9::factory();
}

Ioss::Quad9::Quad9() : Ioss::ElementTopology(Ioss::Quad9::name, "Quadrilateral_9")
{
  Ioss::ElementTopology::alias(Ioss::Quad9::name, "Solid_Quad_9_2D");
  Ioss::ElementTopology::alias(Ioss::Quad9::name, "QUADRILATERAL_9_2D");
  Ioss::ElementTopology::alias(Ioss::Quad9::name, "Face_Quad_9_3D");
  Ioss::ElementTopology::alias(Ioss::Quad9::name, "quadface9");
}

int Ioss::Quad9::parametric_dimension() const { return 2; }
int Ioss::Quad9::spatial_dimension() const { return 2; }
int Ioss::Quad9::order() const { return 2; }

int Ioss::Quad9::number_corner_nodes() const { return 4; }
int Ioss::Quad9::number_nodes() const { return Constants::nnode; }
int Ioss::Quad9::number_edges() const { return Constants::nedge; }
int Ioss::Quad9::number_faces() const { return Constants::nface; }

int Ioss::Quad9::number_nodes_edge(int /* edge */) const { return Constants::nedgenode; }

int Ioss::Quad9::number_nodes_face(int /* face */) const { return 0; }
int Ioss::Quad9::number_edges_face(int /* face */) const { return 0; }

Ioss::IntVector Ioss::Quad9::edge_connectivity(int edge_number) const
{
  assert(edge_number > 0 && edge_number <= number_edges());
  Ioss::IntVector connectivity(Constants::nedgenode);
  assert(edge_number > 0 && edge_number <= Constants::nedge);

  for (int i = 0; i < Constants::nedgenode; i++) {
    connectivity[i] = Constants::edge_node_order[edge_number - 1][i];
  }

  return connectivity;
}

Ioss::IntVector Ioss::Quad9::face_connectivity(int /* face_number */) const
{
  Ioss::IntVector connectivity;
  return connectivity;
}

Ioss::IntVector Ioss::Quad9::element_connectivity() const
{
  Ioss::IntVector connectivity(number_nodes());
  for (int i = 0; i < number_nodes(); i++) {
    connectivity[i] = i;
  }
  return connectivity;
}

Ioss::ElementTopology *Ioss::Quad9::face_type(int /* face_number */) const
{
  return (Ioss::ElementTopology *)nullptr;
}

Ioss::ElementTopology *Ioss::Quad9::edge_type(int edge_number) const
{
  assert(edge_number >= 0 && edge_number <= number_edges());
  IOSS_ASSERT_USED(edge_number);
  return Ioss::ElementTopology::factory("edge3");
}
