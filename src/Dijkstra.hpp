
#pragma once

#include <iostream>
#include "./Polygon.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

namespace ShortestPathLib
{

vector<vector<unsigned int>> AdjList(const Eigen::MatrixXi& edges, unsigned int V);

PolygonalMesh Dijkstra(PolygonalMesh& mesh, const unsigned int& src, const unsigned int& dest);

double findPathLength(PolygonalMesh& mesh);

}
