
/*****************************************************************************
*
* Copyright (c) 2003-2014 by University of Queensland
* http://www.uq.edu.au
*
* Primary Business: Queensland, Australia
* Licensed under the Open Software License version 3.0
* http://www.opensource.org/licenses/osl-3.0.php
*
* Development until 2012 by Earth Systems Science Computational Center (ESSCC)
* Development 2012-2013 by School of Earth Sciences
* Development from 2014 by Centre for Geoscience Computing (GeoComp)
*
*****************************************************************************/

#include <weipa/SpeckleyElements.h>
#include <weipa/NodeData.h>

#ifndef VISIT_PLUGIN
#include <speckley/SpeckleyDomain.h>
#endif

#include <iostream>

#if USE_SILO
#include <silo.h>
#endif


using namespace std;

namespace weipa {
    
//
// Constructor
//
SpeckleyElements::SpeckleyElements(const string& elementName, SpeckleyNodes_ptr nodeData)
    : originalMesh(nodeData), name(elementName), numElements(0),
      numGhostElements(0), nodesPerElement(0),
      type(ZONETYPE_UNKNOWN)
{
    nodeMesh.reset(new SpeckleyNodes(name));
    numGhostElements = 0;
}

//
// Copy constructor
//
SpeckleyElements::SpeckleyElements(const SpeckleyElements& e)
{
    name = e.name;
    numElements = e.numElements;
    numGhostElements = 0;//e.numGhostElements;
    type = e.type;
    nodesPerElement = e.nodesPerElement;
    originalMesh = e.originalMesh;
    if (e.nodeMesh)
        nodeMesh.reset(new SpeckleyNodes(*e.nodeMesh));
    else
        nodeMesh.reset(new SpeckleyNodes(name));

    nodes = e.nodes;
    ID = e.ID;
    //tag = e.tag;
    owner = e.owner;
}

//
//
//
bool SpeckleyElements::initFromSpeckley(const speckley::SpeckleyDomain* dom, int fsType)
{
#ifndef VISIT_PLUGIN
    const pair<int,int> shape = dom->getDataShape(fsType);
    const int* faces = dom->getNumFacesPerBoundary();
    const int* NS = dom->getNumSubdivisionsPerDim();
    const int order = dom->getOrder();

    numElements = shape.second*order*order;
    if (numElements > 0) {
        nodesPerElement = 4;
        if (dom->getDim() == 3) {
            nodesPerElement = 8;
            numElements *= order;
        }
        owner.assign(numElements, dom->getMPIRank());

        const int* iPtr = dom->borrowSampleReferenceIDs(fsType);
        ID.assign(iPtr, iPtr+numElements);

        //iPtr = dom->borrowListOfTags(fsType);
        //tag.assign(iPtr, iPtr+numElements);

        const int* NE = dom->getNumElementsPerDim();
        const int* NN = dom->getNumNodesPerDim();
        nodes.clear();
        if (dom->getDim() == 2) {
            type = ZONETYPE_QUAD;
            if (fsType==speckley::Elements) {
                if (faces[0]==0) {
                    owner[0]=(faces[2]==0 ? dom->getMPIRank()-NS[0]-1
                            : dom->getMPIRank()-1);
                    for (int i=1; i<NE[1]; i++)
                        owner[i*NE[0]]=dom->getMPIRank()-1;
                }
                if (faces[2]==0) {
                    const int first=(faces[0]==0 ? 1 : 0);
                    for (int i=first; i<NE[0]; i++)
                        owner[i]=dom->getMPIRank()-NS[0];
                }
                for (int ey = 0; ey < NE[1]; ey++) {
                    for (int ex = 0; ex < NE[0]; ex++) {
                        int start = order*(ex + ey*NN[0]);
                        for (int qy=0; qy < order; qy++) {
                            int rowstart = start + qy*NN[0];
                            for (int qx=0; qx < order; qx++) {
                                nodes.push_back(rowstart + qx);
                                nodes.push_back(rowstart + qx + 1);
                                nodes.push_back(rowstart + qx + NN[0] + 1);
                                nodes.push_back(rowstart + qx + NN[0]);
                            }
                        }
                    }
                }
            } else if (fsType==speckley::FaceElements) {
                if (faces[0]==0) {
                    if (faces[2]>0)
                        owner[faces[1]]=dom->getMPIRank()-1;
                    if (faces[3]>0)
                        owner[faces[1]+faces[2]]=dom->getMPIRank()-1;
                }
                if (faces[2]==0) {
                    if (faces[0]>0)
                        owner[0]=dom->getMPIRank()-NS[0];
                    if (faces[1]>0)
                        owner[faces[0]]=dom->getMPIRank()-NS[0];
                }

                int id=0;
                for (int i=0; i<faces[0]; i++) {
                    nodes.push_back(id);
                    nodes.push_back(id+NN[0]);
                    id+=NN[0];
                }
                id=NN[0]-1;
                for (int i=0; i<faces[1]; i++) {
                    nodes.push_back(id);
                    nodes.push_back(id+NN[0]);
                    id+=NN[0];
                }
                id=0;
                for (int i=0; i<faces[2]; i++) {
                    nodes.push_back(id);
                    nodes.push_back(id+1);
                    id++;
                }
                id=NN[0]*(NN[1]-1);
                for (int i=0; i<faces[3]; i++) {
                    nodes.push_back(id);
                    nodes.push_back(id+1);
                    id++;
                }
} else {
    std::cerr << "rectangle fallthrough, not elements or faceelements\n";
}
        } else {
            type = ZONETYPE_HEX;
            if (fsType==speckley::Elements) {
                // ownership is not entirely correct but that is not critical.
                // fix when there is time.
                if (faces[1]==0) {
                    for (int k2=0; k2<NE[2]; k2++) {
                        for (int k1=0; k1<NE[1]; k1++) {
                            const int e=k2*NE[0]*NE[1]+(k1+1)*NE[0]-1;
                            owner[e]=dom->getMPIRank()+1;
                        }
                    }
                }
                if (faces[3]==0) {
                    for (int k2=0; k2<NE[2]; k2++) {
                        for (int k0=0; k0<NE[0]; k0++) {
                            const int e=(k2+1)*NE[0]*NE[1]-NE[0]+k0;
                            owner[e]=dom->getMPIRank()+NS[0];
                        }
                    }
                }
                if (faces[5]==0) {
                    for (int k1=0; k1<NE[1]; k1++) {
                        for (int k0=0; k0<NE[0]; k0++) {
                            const int e=k1*NE[0]+k0+NE[0]*NE[1]*(NE[2]-1);
                            owner[e]=dom->getMPIRank()+NS[0]*NS[1];
                        }
                    }
                }
                
                for (int ez = 0; ez < NE[2]; ez++) {
                    for (int ey = 0; ey < NE[1]; ey++) {
                        for (int ex = 0; ex < NE[0]; ex++) {
                            int start = order*(ex + ey*NN[0] + ez*NN[0]*NN[1]);
                            for (int qz = 0; qz < order; qz++) {
                                for (int qy=0; qy < order; qy++) {
                                    for (int qx=0; qx < order; qx++) {
                                        int xstart = start + qy*NN[0] + qz*NN[0]*NN[1] + qx;
                                        nodes.push_back(xstart);
                                        nodes.push_back(xstart + NN[0]*NN[1]);
                                        nodes.push_back(xstart + NN[0]*NN[1] + 1);
                                        nodes.push_back(xstart + 1);
                                        
                                        nodes.push_back(xstart + NN[0]);
                                        nodes.push_back(xstart + NN[0]*(NN[1]+1));
                                        nodes.push_back(xstart + NN[0]*(NN[1]+1)+1);
                                        nodes.push_back(xstart + NN[0]+1);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (fsType==speckley::FaceElements) {
                // ownership is not entirely correct but that is not critical.
                // fix when there is time.
                const int* NE = dom->getNumElementsPerDim();
                int offset=0;
                if (faces[0]>0) {
                    if (faces[3]==0) {
                        for (int k2=0; k2<NE[2]; k2++)
                            owner[(k2+1)*NE[1]-1]=dom->getMPIRank()+NS[0];
                    }
                    if (faces[5]==0) {
                        for (int k1=0; k1<NE[1]; k1++)
                            owner[NE[1]*(NE[2]-1)+k1]=dom->getMPIRank()+NS[0]*NS[1];
                    }
                    for (int k2=0; k2<NE[2]; k2++) {
                        for (int k1=0; k1<NE[1]; k1++) {
                            const int first=k2*NN[0]*NN[1]+k1*NN[0];
                            nodes.push_back(first+NN[0]*NN[1]);
                            nodes.push_back(first);
                            nodes.push_back(first+NN[0]);
                            nodes.push_back(first+NN[0]*(NN[1]+1));
                        }
                    }
                    offset+=faces[0];
                }
                if (faces[1]>0) {
                    if (faces[3]==0) {
                        for (int k2=0; k2<NE[2]; k2++)
                            owner[offset+(k2+1)*NE[1]-1]=dom->getMPIRank()+NS[0]+1;
                    }
                    if (faces[5]==0) {
                        for (int k1=0; k1<NE[1]; k1++)
                            owner[offset+NE[1]*(NE[2]-1)+k1]=dom->getMPIRank()+NS[0]*NS[1]+1;
                    }
                    for (int k2=0; k2<NE[2]; k2++) {
                        for (int k1=0; k1<NE[1]; k1++) {
                            const int first=k2*NN[0]*NN[1]+(k1+1)*NN[0]-1;
                            nodes.push_back(first+NN[0]*NN[1]);
                            nodes.push_back(first);
                            nodes.push_back(first+NN[0]);
                            nodes.push_back(first+NN[0]*(NN[1]+1));
                        }
                    }
                    offset+=faces[1];
                }
                if (faces[2]>0) {
                    if (faces[1]==0) {
                        for (int k2=0; k2<NE[2]; k2++)
                            owner[offset+(k2+1)*NE[0]-1]=dom->getMPIRank()+1;
                    }
                    if (faces[5]==0) {
                        for (int k0=0; k0<NE[0]; k0++)
                            owner[offset+NE[0]*(NE[2]-1)+k0]=dom->getMPIRank()+NS[0]*NS[1];
                    }
                    for (int k2=0; k2<NE[2]; k2++) {
                        for (int k0=0; k0<NE[0]; k0++) {
                            const int first=k2*NN[0]*NN[1]+k0;
                            nodes.push_back(first+NN[0]*NN[1]);
                            nodes.push_back(first);
                            nodes.push_back(first+1);
                            nodes.push_back(first+1+NN[0]*NN[1]);
                        }
                    }
                    offset+=faces[2];
                }
                if (faces[3]>0) {
                    if (faces[1]==0) {
                        for (int k2=0; k2<NE[2]; k2++)
                            owner[offset+(k2+1)*NE[0]-1]=dom->getMPIRank()+NS[0]+1;
                    }
                    if (faces[5]==0) {
                        for (int k0=0; k0<NE[0]; k0++)
                            owner[offset+NE[0]*(NE[2]-1)+k0]=dom->getMPIRank()+NS[0]*(NS[1]+1);
                    }
                    for (int k2=0; k2<NE[2]; k2++) {
                        for (int k0=0; k0<NE[0]; k0++) {
                            const int first=(k2+1)*NN[0]*NN[1]-NN[0]+k0;
                            nodes.push_back(first+NN[0]*NN[1]);
                            nodes.push_back(first);
                            nodes.push_back(first+1);
                            nodes.push_back(first+1+NN[0]*NN[1]);
                        }
                    }
                    offset+=faces[3];
                }
                if (faces[4]>0) {
                    if (faces[1]==0) {
                        for (int k1=0; k1<NE[1]; k1++)
                            owner[offset+(k1+1)*NE[0]-1]=dom->getMPIRank()+1;
                    }
                    if (faces[3]==0) {
                        for (int k0=0; k0<NE[0]; k0++)
                            owner[offset+NE[0]*(NE[1]-1)+k0]=dom->getMPIRank()+NS[0];
                    }
                    for (int k1=0; k1<NE[1]; k1++) {
                        for (int k0=0; k0<NE[0]; k0++) {
                            const int first=k1*NN[0]+k0;
                            nodes.push_back(first);
                            nodes.push_back(first+1);
                            nodes.push_back(first+NN[0]+1);
                            nodes.push_back(first+NN[0]);
                        }
                    }
                    offset+=faces[4];
                }
                if (faces[5]>0) {
                    if (faces[1]==0) {
                        for (int k1=0; k1<NE[1]; k1++)
                            owner[offset+(k1+1)*NE[0]-1]=dom->getMPIRank()+NS[0]*NS[1]+1;
                    }
                    if (faces[3]==0) {
                        for (int k0=0; k0<NE[0]; k0++)
                            owner[offset+NE[0]*(NE[1]-1)+k0]=dom->getMPIRank()+NS[0]*(NS[1]+1);
                    }
                    for (int k1=0; k1<NE[1]; k1++) {
                        for (int k0=0; k0<NE[0]; k0++) {
                            const int first=NN[0]*NN[1]*(NN[2]-1)+k1*NN[0]+k0;
                            nodes.push_back(first);
                            nodes.push_back(first+1);
                            nodes.push_back(first+NN[0]+1);
                            nodes.push_back(first+NN[0]);
                        }
                    }
                }
} else {
    std::cerr << "brick fell through, nothing added\n";
}
        }

        buildMeshes();
    }
    return true;

#else // VISIT_PLUGIN
std::cerr << "visit plugin defined, returning false\n";
    return false;
#endif
}

StringVec SpeckleyElements::getMeshNames() const
{
    StringVec res;
    if (nodeMesh)
        res.push_back(nodeMesh->getName());
    return res;
}

StringVec SpeckleyElements::getVarNames() const
{
    StringVec res;
    res.push_back(name + string("_Id"));
    res.push_back(name + string("_Owner"));
    //res.push_back(name + string("_Tag"));
    return res;
}

const IntVec& SpeckleyElements::getVarDataByName(const string varName) const
{
    if (varName == name+string("_Id"))
        return ID;
    if (varName == name+string("_Owner"))
        return owner;
    //if (varName == name+string("_Tag"))
    //    return tag;

    throw "Invalid variable name";
}

void SpeckleyElements::reorderArray(IntVec& v, const IntVec& idx,
                               int elementsPerIndex)
{
    IntVec newArray(v.size());
    IntVec::iterator arrIt = newArray.begin();
    IntVec::const_iterator idxIt;
    if (elementsPerIndex == 1) {
        for (idxIt=idx.begin(); idxIt!=idx.end(); idxIt++) {
            *arrIt++ = v[*idxIt];
        }
    } else {
        for (idxIt=idx.begin(); idxIt!=idx.end(); idxIt++) {
            int i = *idxIt;
            copy(&v[i*elementsPerIndex], &v[(i+1)*elementsPerIndex], arrIt);
            arrIt += elementsPerIndex;
        }
    }
    v.swap(newArray);
}

IntVec SpeckleyElements::prepareGhostIndices(int ownIndex)
{
    IntVec indexArray;
    numGhostElements = 0;
    
    // move indices of "ghost zones" to the end to be able to reorder
    // data accordingly
    for (int i=0; i<numElements; i++) {
        if (owner[i] == ownIndex)
            indexArray.push_back(i);
    }

    for (int i=0; i<numElements; i++) {
        if (owner[i] != ownIndex) {
            numGhostElements++;
            indexArray.push_back(i);
        }
    }
    return indexArray;
}

void SpeckleyElements::reorderGhostZones(int ownIndex)
{
    IntVec indexArray = prepareGhostIndices(ownIndex);

    // move "ghost data" to the end of the arrays
    if (numGhostElements > 0) {
        reorderArray(nodes, indexArray, nodesPerElement);
        reorderArray(owner, indexArray, 1);
        reorderArray(ID, indexArray, 1);
        //reorderArray(tag, indexArray, 1);
    }
}

void SpeckleyElements::removeGhostZones(int ownIndex)
{
    reorderGhostZones(ownIndex);

    if (numGhostElements > 0) {
        numElements -= numGhostElements;
        nodes.resize(numElements*nodesPerElement);
        owner.resize(numElements);
        ID.resize(numElements);
        //tag.resize(numElements);
        numGhostElements = 0;
    }
}

void SpeckleyElements::buildMeshes()
{
    // build a new mesh containing only the required nodes
    if (numElements > 0) {
        if (nodeMesh && nodeMesh->getNumNodes() > 0) {
            SpeckleyNodes_ptr newMesh(new SpeckleyNodes(nodeMesh, nodes, name));
            nodeMesh.swap(newMesh);
        } else {
            nodeMesh.reset(new SpeckleyNodes(originalMesh, nodes, name));
        }
#ifdef _DEBUG
        cout << nodeMesh->getName() << " has " << nodeMesh->getNumNodes()
            << " nodes and " << numElements << " elements" << endl;
#endif
    }
}

void SpeckleyElements::writeConnectivityVTK(ostream& os)
{
    if (numElements > 0) {
        const IntVec& gNI = nodeMesh->getGlobalNodeIndices();
        IntVec::const_iterator it;
        int count = 1;
        for (it=nodes.begin(); it!=nodes.end(); it++, count++) {
            os << gNI[*it];
            if (count % nodesPerElement == 0)
                os << endl;
            else
                os << " ";
        }
    }
}

#if USE_SILO
inline int toSiloElementType(int type)
{
    switch (type) {
        case ZONETYPE_BEAM: return DB_ZONETYPE_BEAM;
        case ZONETYPE_HEX: return DB_ZONETYPE_HEX;
        case ZONETYPE_POLYGON: return DB_ZONETYPE_POLYGON;
        case ZONETYPE_QUAD: return DB_ZONETYPE_QUAD;
    }
    return 0;
}
#endif

bool SpeckleyElements::writeToSilo(DBfile* dbfile, const string& siloPath,
                                 const StringVec& labels,
                                 const StringVec& units, bool writeMeshData)
{
#if USE_SILO
    if (numElements == 0)
        return true;

    int ret;

    if (siloPath != "") {
        ret = DBSetDir(dbfile, siloPath.c_str());
        if (ret != 0)
            return false;
    }

    // write out the full mesh in any case
    nodeMesh->setSiloPath(siloPath);
    string siloMeshNameStr = nodeMesh->getFullSiloName();
    const char* siloMeshName = siloMeshNameStr.c_str();
    int arraylen = numElements * nodesPerElement;
    int eltype = toSiloElementType(type);

    string varName = name + string("_zones");
    ret = DBPutZonelist2(dbfile, varName.c_str(), numElements,
            nodeMesh->getNumDims(), &nodes[0], arraylen, 0, 0,
            numGhostElements, &eltype, &nodesPerElement, &numElements, 1, NULL);

    if (ret == 0) {
        CoordArray& coordbase = const_cast<CoordArray&>(nodeMesh->getCoords());
        DBoptlist* optList = NULL;
        int nOpts = labels.size()+units.size();
        if (nOpts>0) {
            optList = DBMakeOptlist(nOpts);
            if (labels.size()>0)
                DBAddOption(optList, DBOPT_XLABEL, (void*)labels[0].c_str());
            if (labels.size()>1)
                DBAddOption(optList, DBOPT_YLABEL, (void*)labels[1].c_str());
            if (labels.size()>2)
                DBAddOption(optList, DBOPT_ZLABEL, (void*)labels[2].c_str());
            if (units.size()>0)
                DBAddOption(optList, DBOPT_XUNITS, (void*)units[0].c_str());
            if (units.size()>1)
                DBAddOption(optList, DBOPT_YUNITS, (void*)units[1].c_str());
            if (units.size()>2)
                DBAddOption(optList, DBOPT_ZUNITS, (void*)units[2].c_str());
        }
        ret = DBPutUcdmesh(dbfile, siloMeshName,
                nodeMesh->getNumDims(), NULL, &coordbase[0],
                nodeMesh->getNumNodes(), numElements, varName.c_str(),
                /*"facelist"*/NULL, DB_FLOAT, optList);

        if (optList)
            DBFreeOptlist(optList);
    }
    
    if (ret != 0)
        return false;

    // write out the element-centered variables if enabled
    if (writeMeshData) {
        varName = name + string("_Id");
        ret = DBPutUcdvar1(dbfile, varName.c_str(), siloMeshName,
                (float*)&ID[0], numElements, NULL, 0, DB_INT, DB_ZONECENT,
                NULL);
        if (ret == 0) {
            varName = name + string("_Owner");
            ret = DBPutUcdvar1(dbfile, varName.c_str(), siloMeshName,
                (float*)&owner[0], numElements, NULL, 0, DB_INT, DB_ZONECENT,
                NULL);
        }
    }

    // "Elements" is a special case
    if (writeMeshData && name == "Elements") {
        nodeMesh->writeToSilo(dbfile);
    }

    return (ret == 0);

#else // !USE_SILO
    return false;
#endif
}

} // namespace weipa
