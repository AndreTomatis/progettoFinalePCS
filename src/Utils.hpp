
#pragma once

#include <iostream>
#include "./Polygon.hpp"

using namespace std;

namespace PolygonalLibrary
{
/// Import the triangular mesh and test if the mesh is correct
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportMesh(PolygonalMesh& mesh, string filename);

/// Import the Cell0D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(PolygonalMesh& mesh, string filename);

/// Import the Cell1D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PolygonalMesh& mesh, string filename);

/// Import the Cell2D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PolygonalMesh& mesh, string filename);

/// Import the Cell2D properties from file
/// mesh: a TriangularMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell3Ds(PolygonalMesh& mesh, string filename);

bool check_arguments(unsigned int p, unsigned int q, unsigned int b, unsigned int c);
}
