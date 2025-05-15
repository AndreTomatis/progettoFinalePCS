#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumCell0Ds;
    unsigned int NumCell1Ds;
    unsigned int NumCell2Ds;
    unsigned int NumCell3Ds;

    vector<unsigned int> Cell0DsId;
    vector<unsigned int> Cell1DsId;
    vector<unsigned int> Cell2DsId;
    vector<unsigned int> Cell3DsId;

    Eigen::MatrixXd Cell0DsCoordinates;
    Eigen::MatrixXi Cell1DsExtrema;
    vector<vector<unsigned int>> Cell2DsVertices;
    vector<vector<unsigned int>> Cell2DsEdges;
    vector<vector<unsigned int>> Cell3DsVertices;
    vector<vector<unsigned int>> Cell3DsEdges;
    vector<vector<unsigned int>> Cell3DsFaces;

};

}