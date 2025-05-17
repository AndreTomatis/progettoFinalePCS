
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
  ASSERT_EQ(imp3, true);
}

TEST_F(PolygonalMeshTest, NonNullEdges){
    
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell1DsExtrema.cols(); ++j) {
            
            bool zero_length = (polygons[i].Cell1DsExtrema(0,i) == polygons[i].Cell1DsExtrema(1,i));
            
            EXPECT_EQ(zero_length, false);
        }
    }
    
}


TEST_F(PolygonalMeshTest, NonNullFaces){
    for (unsigned int i = 0; i < 3; i++) {
        ;//cose
    }
}


//TEST per controllare che tutte le figure siano inscritte nella circonferenza
TEST_F(PolygonalMeshTest, InCircumference){
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell0DsCoordinates.cols(); ++j) {
            float module = pow(polygons[i].Cell0DsCoordinates(0,i), 2) + pow(polygons[i].Cell0DsCoordinates(1,i),2) + pow(polygons[i].Cell0DsCoordinates(2,i),2);
            EXPECT_EQ(module, 1);
        }
    }
}


}  // SortLibrary
