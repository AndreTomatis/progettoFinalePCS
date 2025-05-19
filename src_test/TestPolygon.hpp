
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
            
            bool zero_length = (polygons[i].Cell1DsExtrema(0,j) == polygons[i].Cell1DsExtrema(1,j));
            
            EXPECT_EQ(zero_length, false);
        }
    }
}

//test per vedere se tutti gli edges sono lunghi uguali
TEST_F(PolygonalMeshTest, SameLengthEdges){
    
    for (unsigned int i = 0; i < 3; i++){
        auto id1 = polygons[i].Cell1DsExtrema(0,0),
             id2 = polygons[i].Cell1DsExtrema(1,0);
            
        double ref_length = (polygons[i].Cell0DsCoordinates.col(id1) - polygons[i].Cell0DsCoordinates.col(id2)).norm();
        for (unsigned int j = 0; j < polygons[i].Cell1DsExtrema.cols(); ++j) {
            id1 = polygons[i].Cell1DsExtrema(0,j),
            id2 = polygons[i].Cell1DsExtrema(1,j);
            double cmp_length = (polygons[i].Cell0DsCoordinates.col(id1) - polygons[i].Cell0DsCoordinates.col(id2)).norm();

            if (ref_length - cmp_length >= 0.01){
                cout << i << j << " " << ref_length << " " << cmp_length << endl;
            }

            EXPECT_EQ(ref_length - cmp_length < 0.01, true);
        }
    }
}



TEST_F(PolygonalMeshTest, NonNullFaces){
    for (unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < polygons[i].Cell2DsVertices.size(); j++){
            Eigen::Vector3d area_vector(0, 0, 0);
            for(unsigned int k =0; k < polygons[i].Cell2DsVertices[j].size(); k++){
                auto id1 =  polygons[i].Cell2DsVertices[j][k];
                auto id2 =  polygons[i].Cell2DsVertices[j][(k+1) % polygons[i].Cell2DsVertices[j].size()];
                const Eigen::Vector3d& v1= polygons[i].Cell0DsCoordinates.col(id1);
                const Eigen::Vector3d& v2= polygons[i].Cell0DsCoordinates.col(id2);
                area_vector += v1.cross(v2);
            }
            bool null_area = (0.5 * area_vector.norm()) < 1e-16;
            EXPECT_EQ(null_area, false);
        }
    }
}


//TEST per controllare che tutte le figure siano inscritte nella circonferenza
TEST_F(PolygonalMeshTest, InCircumference){
    for (unsigned int i = 0; i < 3; i++){
        for (unsigned int j = 0; j < polygons[i].Cell0DsCoordinates.cols(); ++j) {
            float module = pow(polygons[i].Cell0DsCoordinates(0,j), 2) + pow(polygons[i].Cell0DsCoordinates(1,j),2) + pow(polygons[i].Cell0DsCoordinates(2,j),2);
            bool valid = abs(module - 1.0) < 1e-6;
            
            EXPECT_EQ(valid, true);
        }
    }
}


}  // SortLibrary
