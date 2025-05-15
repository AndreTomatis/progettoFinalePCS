
#pragma once

#include <iostream>
#include "./PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary
{
/// Import the triangular mesh and test if the mesh is correct
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportMesh(PolygonalMesh& mesh);

/// Import the Cell0D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(PolygonalMesh& mesh);

/// Import the Cell1D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PolygonalMesh& mesh);

/// Import the Cell2D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PolygonalMesh& mesh);

/// Import the Cell2D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell3Ds(PolygonalMesh& mesh);

// Store the markers in a map with "marker code : list of element with that marker" as key:value
void importMarker(map<unsigned int, list<unsigned int>>& m, int marker, int id);

}
