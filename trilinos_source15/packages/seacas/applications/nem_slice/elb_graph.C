/*
 * Copyright(C) 1999-2023 National Technology & Engineering Solutions
 * of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
 * NTESS, the U.S. Government retains certain rights in this software.
 *
 * See packages/seacas/LICENSE for details
 */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Functions contained in this file:
 *      generate_graph()
 *      find_surnd_elems()
 *      find_adjacency()
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "copy_string_cpp.h"
#include "elb.h"      // for Problem_Description, etc
#include "elb_elem.h" // for get_elem_info, NNODES, etc
#include "elb_err.h"  // for Gen_Error
#include "elb_graph.h"
#include "elb_util.h" // for in_list, find_inter
#include <cassert>    // for assert
#include <cstddef>    // for size_t
#include <cstdlib>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <sstream>
#include <vector> // for vector

/* Local function prototypes */
namespace {
  template <typename INT>
  int find_surnd_elems(Mesh_Description<INT> * /*mesh*/, Graph_Description<INT> * /*graph*/);

  template <typename INT>
  int find_adjacency(Problem_Description * /*problem*/, Mesh_Description<INT> * /*mesh*/,
                     Graph_Description<INT> * /*graph*/, Weight_Description<INT> * /*weight*/,
                     Sphere_Info * /*sphere*/);
} // namespace
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Function generate_graph() begins:
 *----------------------------------------------------------------------------
 * This function does the work to generate the graph from the FE mesh.
 *****************************************************************************/
template int generate_graph(Problem_Description *problem, Mesh_Description<int> *mesh,
                            Graph_Description<int> *graph, Weight_Description<int> *weight,
                            Sphere_Info *sphere);

template int generate_graph(Problem_Description *problem, Mesh_Description<int64_t> *mesh,
                            Graph_Description<int64_t> *graph, Weight_Description<int64_t> *weight,
                            Sphere_Info *sphere);

template <typename INT>
int generate_graph(Problem_Description *problem, Mesh_Description<INT> *mesh,
                   Graph_Description<INT> *graph, Weight_Description<INT> *weight,
                   Sphere_Info *sphere)
{
  double time1 = get_time();
  /* Find the elements surrounding a node */
  if (!find_surnd_elems(mesh, graph)) {
    Gen_Error(0, "fatal: could not find surrounding elements");
    return 0;
  }
  double time2 = get_time();
  fmt::print(stderr, "Time to find surrounding elements: {}s\n", time2 - time1);

  /* Find the adjacency, if required */
  if (problem->alloc_graph == ELB_TRUE) {
    if (!find_adjacency(problem, mesh, graph, weight, sphere)) {
      Gen_Error(0, "fatal: could not find adjacency");
      return 0;
    }
    time1 = get_time();
    fmt::print(stderr, "Time to find the adjacency: {}s\n", time2 - time1);
  }
  return 1;
}

namespace {
  /*****************************************************************************/
  /*****************************************************************************/
  /*****************************************************************************/
  /* Function find_surnd_elems() begins:
   *----------------------------------------------------------------------------
   * This function finds the elements surrounding a given FEM node. In other
   * words, this function generates a list of elements containing a given
   * FEM node.
   *****************************************************************************/
  template <typename INT>
  int find_surnd_elems(Mesh_Description<INT> *mesh, Graph_Description<INT> *graph)
  {
    std::vector<int>    surround_count(mesh->num_nodes);
    std::vector<size_t> last_element(mesh->num_nodes);

    size_t sur_elem_total_size = 0;
    /* Find the count of surrounding elements for each node in the mesh */
    // The hope is that this code will speed up the entire routine even
    // though we are iterating the complete connectivity array twice.
    for (size_t ecnt = 0; ecnt < mesh->num_elems; ecnt++) {
      int nnodes = get_elem_info(NNODES, mesh->elem_type[ecnt]);
      for (int ncnt = 0; ncnt < nnodes; ncnt++) {
        INT node = mesh->connect[ecnt][ncnt];
        assert(node < (INT)mesh->num_nodes);
        /*
         * in the case of degenerate elements, where a node can be
         * entered into the connect table twice, need to check to
         * make sure that this element is not already listed as
         * surrounding this node
         */
        if (last_element[node] != ecnt + 1) {
          last_element[node] = ecnt + 1;
          surround_count[node]++;
          sur_elem_total_size++;
        }
      }
    } /* End "for(ecnt=0; ecnt < mesh->num_elems; ecnt++)" */

    size_t v_size  = sizeof(std::vector<INT>);
    size_t vv_size = sizeof(std::vector<std::vector<INT>>);
    size_t total   = vv_size + mesh->num_nodes * v_size + sur_elem_total_size * sizeof(INT);
    fmt::print(stderr, "\ttotal size of reverse connectivity array: {} entries ({} bytes).\n",
               fmt::group_digits(sur_elem_total_size), fmt::group_digits(total));
    vec_free(last_element);

    // Attempt to reserve an array with this size...
    double time1 = get_time();

    graph->sur_elem.resize(mesh->num_nodes);
    for (size_t ncnt = 0; ncnt < mesh->num_nodes; ncnt++) {
      if (surround_count[ncnt] == 0) {
        fmt::print(stderr, "WARNING: Node = {} has no elements\n", ncnt + 1);
      }
      else {
        graph->sur_elem[ncnt].reserve(surround_count[ncnt]);
        graph->max_nsur =
            surround_count[ncnt] > graph->max_nsur ? surround_count[ncnt] : graph->max_nsur;
      }
    }
    double time2 = get_time();

    fmt::print(stderr,
               "\tmemory allocated...({} seconds)\n"
               "\tmax of {} elements per node\n",
               time2 - time1, graph->max_nsur);

    /* Find the surrounding elements for each node in the mesh */
    for (size_t ecnt = 0; ecnt < mesh->num_elems; ecnt++) {
      int nnodes = get_elem_info(NNODES, mesh->elem_type[ecnt]);
      for (int ncnt = 0; ncnt < nnodes; ncnt++) {
        INT node = mesh->connect[ecnt][ncnt];

        /*
         * in the case of degenerate elements, where a node can be
         * entered into the connect table twice, need to check to
         * make sure that this element is not already listed as
         * surrounding this node
         */
        if (graph->sur_elem[node].empty() || ecnt != (size_t)graph->sur_elem[node].back()) {
          /* Add the element to the list */
          graph->sur_elem[node].push_back(ecnt);
        }
      }
    } /* End "for(ecnt=0; ecnt < mesh->num_elems; ecnt++)" */

#ifndef NDEBUG
    for (size_t ncnt = 0; ncnt < mesh->num_nodes; ncnt++) {
      assert(graph->sur_elem[ncnt].size() == (size_t)surround_count[ncnt]);
    }
#endif
    return 1;
  }

  /*****************************************************************************/
  /*****************************************************************************/
  /*****************************************************************************/
  /* Function find_adjacency() begins:
   *----------------------------------------------------------------------------
   * This function finds adjacency (or graph) of the problem.
   *****************************************************************************/
  template <typename INT>
  int find_adjacency(Problem_Description *problem, Mesh_Description<INT> *mesh,
                     Graph_Description<INT> *graph, Weight_Description<INT> *weight,
                     Sphere_Info *sphere)
  {
    std::vector<INT> pt_list;
    std::vector<INT> hold_elem;
    INT              side_nodes[MAX_SIDE_NODES + 2];
    INT              mirror_nodes[MAX_SIDE_NODES + 2];

    static int count = 0;

    int hflag1;
    int hflag2;
    int tflag1;
    int tflag2;
    /*-----------------------------Execution Begins------------------------------*/

    /* Allocate memory necessary for the adjacency */
    graph->start.resize(problem->num_vertices + 1);

    for (int i = 0; i < MAX_SIDE_NODES + 2; i++) {
      side_nodes[i]   = -999;
      mirror_nodes[i] = -999;
    }

    /* Find the adjacency for a nodal based decomposition */
    if (problem->type == NODAL) {
      graph->nadj = 0;
      for (size_t ncnt = 0; ncnt < mesh->num_nodes; ncnt++) {
        graph->start[ncnt] = graph->nadj;
        assert(graph->nadj == graph->adj.size());
        for (size_t ecnt = 0; ecnt < graph->sur_elem[ncnt].size(); ecnt++) {
          size_t elem   = graph->sur_elem[ncnt][ecnt];
          int    nnodes = get_elem_info(NNODES, mesh->elem_type[elem]);
          for (int i = 0; i < nnodes; i++) {
            INT entry = mesh->connect[elem][i];

            if (ncnt != (size_t)entry && in_list(entry, graph->adj.size() - graph->start[ncnt],
                                                 &graph->adj[graph->start[ncnt]]) < 0) {
              graph->adj.push_back(entry);
              graph->nadj++;
            }
          }
        } /* End "for(ecnt=0; ecnt < graph->nsur_elem[ncnt]; ecnt++)" */
      }   /* End "for(ncnt=0; ncnt < mesh->num_nodes; ncnt++)" */
    }
    /* Find the adjacency for a elemental based decomposition */
    else {

      /* for face adjacencies, need to allocate some memory */
      if (problem->face_adj) {
        /* allocate space to hold info about surrounding elements */
        pt_list.resize(graph->max_nsur);
        hold_elem.resize(graph->max_nsur);
      }
      graph->nadj = 0;
      size_t cnt  = 0;

      /* tmp_element used to speed up the in_list calc */
      std::vector<int> tmp_element(mesh->num_elems, -1);

      /* Cycle through the elements */

      int element_3d = 0;
      int nnodes     = mesh->num_dims;
      int nsides     = 0;

      for (size_t ecnt = 0; ecnt < mesh->num_elems; ecnt++) {
        E_Type etype      = mesh->elem_type[ecnt];
        E_Type etype_last = NULL_EL;
        if (etype != etype_last) {
          etype_last = etype;
          element_3d = is_3d_element(mesh->elem_type[ecnt]);
          if (problem->face_adj == 0) {
            nnodes = get_elem_info(NNODES, etype);
          }
          nsides = get_elem_info(NSIDES, etype);
        }

        if (etype != SPHERE || problem->no_sph == 1) {
          graph->start[cnt] = graph->nadj;
          assert(graph->nadj == graph->adj.size());

          /*
           * now have to decide how to determine adjacency
           * !face_adj - any element that connects to any node in this
           *             element is an adjacent element
           * face_adj - a) for 3D elements only those that share an
           *               entire face with this element are considered
           *               adjacent
           *            b) do not connect 1D/2D elements to 3D elements
           *            c) 1D and 2D elements can connect to each other
           */

          /* If not forcing face adjaceny */
          if (problem->face_adj == 0) {

            /* ncnt = 0,...,7 for hex */
            for (int ncnt = 0; ncnt < nnodes; ncnt++) {
              /* node is the node number 'ncnt' of element 'ecnt' */
              size_t node = mesh->connect[ecnt][ncnt];

              /* i varies from 0 -> # of elements touching 'node' */
              for (size_t i = 0; i < graph->sur_elem[node].size(); i++) {

                /* 'entry' is element # i touching node 'node' */
                INT entry = graph->sur_elem[node][i];

                /* make sure we're not checking if the element
                   is connected to itself */
                if (ecnt != (size_t)entry && mesh->elem_type[entry] != SPHERE) {
                  /* If tmp_element[entry] != ecnt, then entry is not in list... */
                  if ((size_t)tmp_element[entry] != ecnt) {
#if 0
                    assert(in_list(entry, (graph->nadj)-(graph->start[cnt]),
                                   (graph->adj)+(graph->start[cnt])) < 0);
#endif
                    tmp_element[entry] = ecnt;
                    (graph->nadj)++;
                    graph->adj.push_back(entry);
                    if (weight->type & EDGE_WGT) {
                      weight->edges.push_back(1.0);
                    }
                  }
                  else if (weight->type & EDGE_WGT) {
                    int iret = in_list(entry, (graph->nadj) - (graph->start[cnt]),
                                       &graph->adj[graph->start[cnt]]);
                    assert(iret >= 0);
                    weight->edges[iret + (graph->start[cnt])] += 1.0F;
                  }
                }
              }
            } /* End "for(ncnt=0; ...)" */
          }   /* End: "if (problem->face_adj == 0)" */

          /* So if this is a 3-d element and we're forcing face
           * adjacency, if it gets to this else below
           *
           * if this element is 1d/2d allow connections to 1d and 2d
           * elements but not to 3d elements
           *
           */

          else {
            if (element_3d) {
              /* need to check for hex's or tet's */

              /*
               * If the first element is a hex or tet, set flags
               * hflag1/tflag1 to 1
               */
              hflag1 = is_hex(etype);
              tflag1 = is_tet(etype);

              /* check each side of this element */
              for (int nscnt = 0; nscnt < nsides; nscnt++) {
                /* get the list of nodes on this side set */
                int side_cnt = ss_to_node_list(etype, mesh->connect[ecnt], (nscnt + 1), side_nodes);

                /*
                 * now I need to determine how many side set nodes I
                 * need to use to determine if there is an element
                 * connected to this side.
                 *
                 * 2-D - need two nodes, so find one intersection
                 * 3-D - need three nodes, so find two intersections
                 * NOTE: must check to make sure that this number is not
                 *       larger than the number of nodes on the sides (ie - SHELL).
                 */

                nnodes = mesh->num_dims;

                /*
                 * In case the number of nodes on this side are less
                 * than the minimum number, set nnodes to side_cnt,
                 * i.e., if a 3-D mesh contains a bar, then nnodes=3,
                 * and side_cnt = 2
                 */

                if (nnodes > side_cnt) {
                  nnodes = side_cnt;
                }

                nnodes--; /* decrement to find the number of intersections  */

                size_t nelem = 0; /* reset this in case no intersections are needed */

                /* copy the first array into temp storage */

#if 0
                /* nhold is the number of elements touching node 0 on
                   the side of this element */
                size_t nhold = graph->sur_elem[side_nodes[0]].size();

                /* Now that we have the number of elements touching
                   side 0, get their element ids and store them in hold_elem */
                for (size_t ncnt = 0; ncnt < nhold; ncnt++)
                  hold_elem[ncnt] = graph->sur_elem[side_nodes[0]][ncnt];
#endif
                /*
                 * need to handle hex's differently because of
                 * the tet/hex combination
                 */

                if (!hflag1) {
                  /* Get the number of elements ( and their ids )
                     that touch node (ncnt+1) and see if any elements touch
                     both node 0 and node (ncnt+1), and if so, return to nelem
                     the number of elements touching both nodes and their
                     indices in pt_list.  When ncnt != 0, hold_elem and nhold
                     change */
                  size_t nhold = graph->sur_elem[side_nodes[0]].size();
                  for (size_t ncnt = 0; ncnt < nhold; ncnt++) {
                    hold_elem[ncnt] = graph->sur_elem[side_nodes[0]][ncnt];
                  }

                  for (int ncnt = 0; ncnt < nnodes; ncnt++) {
                    nelem = find_inter(
                        hold_elem.data(), &graph->sur_elem[side_nodes[(ncnt + 1)]][0], nhold,
                        graph->sur_elem[side_nodes[(ncnt + 1)]].size(), pt_list.data());

                    /*  If less than 2 ( 0 or 1 ) elements only
                        touch nodes 0 and ncnt+1 then try next side node, i.e.,
                        repeat loop ncnt */
                    if (nelem < 2) {
                      break;
                    }

                    nhold = nelem;
                    for (size_t i = 0; i < nelem; i++) {
                      hold_elem[i] = hold_elem[pt_list[i]];
                    }
                  }
                }

                /* If this element is a hex type */
                else {

                  /*
                   * To handle hex's, check opposite corners. First check
                   * 1 and 3 and then 2 and 4. Only an element connected
                   * to this face will be connected to both corners. If there
                   * are tet's connected to this face, both will show up in
                   * one of the intersections (nothing will show up in the
                   * other intersection).
                   */

                  /* See if hexes share nodes 0 and nodes (ncnt+2) */
                  int inode = 0;
                  for (int ncnt = 0; ncnt < nnodes; ncnt++) {
                    nelem =
                        find_inter(&graph->sur_elem[side_nodes[inode]][0],
                                   &graph->sur_elem[side_nodes[(ncnt + 2)]][0],
                                   graph->sur_elem[side_nodes[inode]].size(),
                                   graph->sur_elem[side_nodes[(ncnt + 2)]].size(), pt_list.data());

                    /*
                     * If there are multiple elements in the intersection, then
                     * they must share the face, since the intersection is between
                     * the corner nodes. No element could connect with both of
                     * those nodes without being connected elsewhere.
                     */
                    if (nelem > 1) {

                      /* Then get the correct elements out of the hold array */
                      for (size_t i = 0; i < nelem; i++) {
                        hold_elem[i] = graph->sur_elem[side_nodes[inode]][pt_list[i]];
                      }
                      break;
                    }

                    /*
                     * if there aren't multiple elements in the intersection,
                     * then check the opposite corners (1 & 3)
                     */
                    inode = 1;
                  }
                } /* "if (!hflag)" */

                /*
                 * if there is an element on this side of ecnt, then there
                 * will be at least two elements in the intersection (one
                 * will be ecnt)
                 */
                if (nelem > 1) {

                  /*
                   * now go through and check each element in the list
                   * to see if it is different than ecnt.
                   */

                  for (size_t i = 0; i < nelem; i++) {
                    size_t entry = hold_elem[i];

                    if (ecnt != entry) {

                      /*
                       * Need to verify that this side of ecnt is actually
                       * connected to a face of entry. The problem case is
                       * when an entire face of a shell (one of the ends)
                       * is connected to only an edge of a quad/tet
                       */

                      E_Type etype2 = mesh->elem_type[entry];

                      /* make sure this is a 3d element*/

                      if (is_3d_element(etype2)) {

                        /* need to check for hex's */
                        hflag2 = is_hex(etype2);

                        /* TET10 cannot connect to a HEX */
                        tflag2 = is_tet(etype2);

                        /* check here for tet/hex combinations */
                        int sid;
                        if ((tflag1 && hflag2) || (hflag1 && tflag2)) {
                          /*
                           * have to call a special function to get the side id
                           * in these cases. In both cases, the number of side
                           * nodes for the element will not be consistent with
                           * side_cnt, and:
                           *
                           * TET/HEX - side_nodes only contains three of the
                           *           the side nodes of the hex.
                           *
                           * HEX/TET - Have to check that this tet shares a side
                           *           with the hex.
                           */
                          sid = get_side_id_hex_tet(mesh->elem_type[entry], mesh->connect[entry],
                                                    side_cnt, side_nodes);
                        }
                        else {
                          /*
                           * get the side id of elem. Make sure that ecnt is
                           * trying to communicate to a valid side of elem
                           */

                          side_cnt = get_ss_mirror(etype, side_nodes, (nscnt + 1), mirror_nodes);

                          /*
                           * small kludge to handle 6 node faces butted up against
                           * 4 node faces
                           */

                          /* if this element 1 is a hexshell, then only
                             require 4 of the 6 nodes to match between elements
                             1 and 2 */
                          if (etype == HEXSHELL && side_cnt == 6) {
                            side_cnt = 4;
                          }

                          /* side_cnt is the number of nodes on the face
                             of a particular element.  This number is passed
                             to get_side_id and the error with two hexes
                             only sharing 3 nodes is in get_side_id
                             Additional comments can be found there */

                          /*
                           * in order to get the correct side order for elem,
                           * get the mirror of the side of ecnt
                           */

                          /* Based on elements intersecting, get the side
                             of element 1 that is connected to the element in the list
                             which it intersects with.  The two elements must have
                             (originally) side_cnt nodes in common */

                          sid =
                              get_side_id(mesh->elem_type[entry], mesh->connect[entry], side_cnt,
                                          mirror_nodes, problem->skip_checks, problem->partial_adj);
                        }

                        if (sid > 0) {
                          (graph->nadj)++;
                          graph->adj.push_back(entry);
                          if (weight->type & EDGE_WGT) {
                            /*
                             * the edge weight is the number of nodes in the
                             * connecting face
                             */
                            weight->edges.push_back(side_cnt);

                            /*
                             * have to put a kluge in here for the
                             * tet/hex problem
                             */
                            if (hflag1 && tflag2) {
                              (weight->edges.back())--;
                            }
                          }
                        }
                        else if ((sid < 0) && (!problem->skip_checks)) {
                          /*
                           * too many errors with bad meshes, print out
                           * more information here for diagnostics
                           */
                          std::string tmpstr;
                          std::string cmesg;
                          cmesg = "Error returned while getting side id for communication map.";
                          Gen_Error(0, cmesg);
                          cmesg = fmt::format("Element 1: {}", (ecnt + 1));
                          Gen_Error(0, cmesg);
                          nnodes = get_elem_info(NNODES, etype);
                          cmesg  = "connect table:";
                          for (int ii = 0; ii < nnodes; ii++) {
                            tmpstr = fmt::format(" {}", (size_t)(mesh->connect[ecnt][ii] + 1));
                            cmesg += tmpstr;
                          }
                          Gen_Error(0, cmesg);
                          cmesg = fmt::format("side id: {}", (nscnt + 1));
                          Gen_Error(0, cmesg);
                          cmesg = "side nodes:";
                          for (int ii = 0; ii < side_cnt; ii++) {
                            tmpstr = fmt::format(" {}", (size_t)(side_nodes[ii] + 1));
                            cmesg += tmpstr;
                          }
                          Gen_Error(0, cmesg);
                          cmesg = fmt::format("Element 2: {}", (entry + 1));
                          Gen_Error(0, cmesg);
                          nnodes = get_elem_info(NNODES, etype2);
                          cmesg  = "connect table:";
                          for (int ii = 0; ii < nnodes; ii++) {
                            tmpstr = fmt::format(" {}", (size_t)(mesh->connect[entry][ii] + 1));
                            cmesg += tmpstr;
                          }
                          Gen_Error(0, cmesg);
                          count++;
                          fmt::print("Now we have {} bad element connections.\n", count);
                        } /* End "if (sid > 0)" */
                      }   /* End: "if(ecnt != entry)" */
                    }
                  } /* End: "for(i=0; i < nelem; i++)" */
                }   /* End: "if (nelem > 1)" */
              }     /* End: "for (nscnt = 0; nscnt < nsides; nscnt++)" */
            }       /* End: "if(element_3d)" */

            else {

              /* this is either a 2d or 1d element. Only allow attachments to other
               * 1d or 2d elements
               */

              nnodes = get_elem_info(NNODES, mesh->elem_type[ecnt]);

              for (int ncnt = 0; ncnt < nnodes; ncnt++) {
                /* node is the node number 'ncnt' of element 'ecnt' */
                size_t node = mesh->connect[ecnt][ncnt];

                /* i varies from 0 -> # of elements touching 'node' */
                for (size_t i = 0; i < graph->sur_elem[node].size(); i++) {

                  /* 'entry' is element # i touching node 'node' */
                  INT entry = graph->sur_elem[node][i];

                  /* make sure we're not checking if the element
                     is connected to itself */
                  if (ecnt != (size_t)entry) {

                    /* now make sure that the entry is not a 3d element */
                    if (!is_3d_element(mesh->elem_type[entry])) {

                      int iret;
                      if ((iret = in_list(entry, graph->adj.size() - graph->start[cnt],
                                          &graph->adj[graph->start[cnt]])) < 0) {

                        (graph->nadj)++;
                        graph->adj.push_back(entry);
                        if (weight->type & EDGE_WGT) {
                          weight->edges.push_back(1.0);
                        }
                      }
                      else if (weight->type & EDGE_WGT) {
                        weight->edges[iret + (graph->start[cnt])] += 1.0F;
                      }
                    }
                  } /* End: if(ecnt != entry) */
                }   /* for(i=0; i < graph->nsur_elem[node]; i++) */
              }     /* End "for(ncnt=0; ...)" */
            }       /* End: "else" (if !element_3d) */
          }         /* End: "else" (if face_adj != 0) */

          cnt++;

        } /* End "if(etype != SPHERE)" */
      }   /* End "for(ecnt=0; ecnt < mesh->num_elems; ecnt++)" */
    }

    graph->start[problem->num_vertices] = graph->adj.size();
    graph->nadj                         = graph->adj.size();

    if ((size_t)graph->start[problem->num_vertices] != graph->nadj) {
      // Possibly an integer overflow... Output error message and stop.
      std::ostringstream errmsg;
      fmt::print(errmsg,
                 "fatal: Graph adjacency edge count ({}) exceeds chaco 32-bit integer range.\n",
                 graph->nadj);
      Gen_Error(0, errmsg.str());
      return 0;
    }

    /* Adjust for a mesh with spheres */
    if (problem->type == ELEMENTAL && sphere->num) {
      /* Decrement adjacency entries */
      for (auto &elem : graph->adj) {
        for (size_t ecnt = 0; ecnt < mesh->num_el_blks; ecnt++) {
          if (elem >= sphere->begin[ecnt] && elem < sphere->end[ecnt]) {
            elem -= sphere->adjust[ecnt];
            break;
          }
        }
      }
    }
    return 1;
  }
} // namespace
