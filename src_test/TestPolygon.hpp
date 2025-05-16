
#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <cmath>
#include "Polygon.hpp"
#include "Utils.hpp"

using namespace PolygonalLibrary;
using namespace std;


namespace TestLibrary{

// The fixture for testing class Foo.
class PolygonalMeshTest : public testing::Test {
 public:
    bool imp1,imp2, imp3;
 protected:

  PolygonalMeshTest() {
    imp1 = ImportMesh(polygons[0], "../Polygons/p3q3/");
    imp2 = ImportMesh(polygons[1], "../Polygons/p4q3/");
    imp3 = ImportMesh(polygons[2], "../Polygons/p5q3/");
  }

  PolygonalMesh polygons[3];
};


TEST_F(PolygonalMeshTest, Imports) {
  ASSERT_EQ(imp1, true);
  ASSERT_EQ(imp2, true);
  //ASSERT_EQ(imp3, true);
}

TEST_F(PolygonalMeshTest, NonNullEdges){
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell1DsExtrema.rows(); ++j) {
            bool zero_length = (polygons[i].Cell1DsExtrema(i,1) == polygons[i].Cell1DsExtrema(i,2));
            EXPECT_EQ(zero_length, false);
        }
    }
}

/*
TEST_F(PolygonalMeshTest, NonNullFaces){
    for (unsigned int i = 0; i < 3; i++) {
        for (unsigned int j = 0; j < polygons[i].Cell2DsVertices.size(); ++j) {
            vector<unsigned int>& polygon_vertices = polygons[i].Cell2DsVertices[j];
            const unsigned int n = polygon_vertices.size();
            const Eigen::MatrixXd& coords = polygons[i].Cell0DsCoordinates;

            Eigen::Vector3d area_vector(0, 0, 0);

            for (unsigned int k = 0; k < n; ++k) {
                unsigned int vi_idx = polygon_vertices[k];
                unsigned int vj_idx = polygon_vertices[(k + 1) % n];

                // Get vertex positions from N x 3 coordinate matrix
                Eigen::Vector3d vi = coords.row(vi_idx);
                Eigen::Vector3d vj = coords.row(vj_idx);

                area_vector += vi.cross(vj);
            }

            bool null_area = 0.5 * area_vector.norm() < 1e-16;
            ASSERT_EQ(null_area, false);
        }
    }
}
*/

//TEST per controllare che tutte le figure siano inscritte nella circonferenza
TEST_F(PolygonalMeshTest, InCircumference){
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell0DsCoordinates.rows(); ++j) {
            float module = pow(polygons[i].Cell0DsCoordinates(i,0), 2) + pow(polygons[i].Cell0DsCoordinates(i,1),2) + pow(polygons[i].Cell0DsCoordinates(i,2),2);
            EXPECT_EQ(module, 1);
        }
    }
}


}  // SortLibrary
