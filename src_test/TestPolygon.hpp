
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

TEST_F(PolygonalMeshTest, NonNullFaces){
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell2DsVertices.size(); ++j) {
            vector<unsigned int>& polygon_vertices = polygons[i].Cell2DsVertices[j];
            const unsigned int vertex_count = polygon_vertices.size();

            bool signed_area;
            const MatrixXd& coords = polygons[i].Cell0DsCoordinates;

            for (size_t v = 0; v < vertex_count; ++v)
            {
                unsigned int idx_a = polygon_vertices[v];
                unsigned int idx_b = polygon_vertices[(v + 1) % vertex_count]; // wrap-around to first

                double xa = coords(0, idx_a);
                double ya = coords(1, idx_a);
                double xb = coords(0, idx_b);
                double yb = coords(1, idx_b);
                signed_area += xa * yb - xb * ya;
            }

            signed_area = fabs(0.5 * signed_area) < 1e-16;
            cout << i << " " << j << endl;
            ASSERT_EQ(signed_area, false);
        }
    }
}

//TEST per controllare che le faccie siano correttamente salvate
//TEST per controllare che tutte le figure siano inscritte nella circonferenza
TEST_F(PolygonalMeshTest, InCircumference){
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell0DsCoordinates.rows(); ++j) {
            float module = pow(polygons[i].Cell1DsExtrema(i,1), 2) + pow(polygons[i].Cell1DsExtrema(i,2),2) + pow(polygons[i].Cell1DsExtrema(i,3),2);
            EXPECT_EQ(module, 1);
        }
    }
}


}  // SortLibrary
