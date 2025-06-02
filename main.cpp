#include <iostream>
#include "Polygon.hpp"
#include "Utils.hpp"
#include "Dijkstra.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;


int main(int argc, char* argv[])
{
    
    string filename = "./Polygons/";
    bool swap = false;
    unsigned int p,q,b,c;


    if (argc == 5 || argc == 7){
        
        p = stoi(argv[1]); q = stoi(argv[2]); b = stoi(argv[3]); c = stoi(argv[4]);
        
        if (!check_arguments(p,q,b,c)){
            cerr << "Invalid values of p, q, b or c" << endl;
            return 1;
        }

        
        
        PolygonalMesh mesh(p,q);


        if(q==3 && p != 3)
        {
            filename += "./p" + to_string(p) + "q" + to_string(q) + "/";
        }


        if (p == 3){
            filename += "./p" + to_string(q) + "q" + to_string(p) + "/";
            mesh.p = q;
            mesh.q = 3;
            swap = true;
        }


        if(!ImportMesh(mesh, filename))
        {
            cerr << "Unable to import the mesh, something went wrong" << endl;
            return 1;
        }


        mesh = mesh.CreateDual();
       

        unsigned int T = b*b + b*c + c*c;

        if (b==c)
            mesh = Triangulation_2(mesh, b, T);
        else 
            mesh = Triangulation_1(mesh, b, T);


        // project the vertices on a circumference
        for(unsigned int i = 0; i < mesh.NumCell0Ds; i++){
            mesh.Cell0DsCoordinates.col(i) /= mesh.Cell0DsCoordinates.col(i).norm();
        }

        

        if (b != c){
            if(!swap){
                cout << "Trying Goldberg" << endl;
                mesh = mesh.CreateDual();
            }

            // project the vertices on a circumference
            for(unsigned int i = 0; i < mesh.NumCell0Ds; i++){
                mesh.Cell0DsCoordinates.col(i) /= mesh.Cell0DsCoordinates.col(i).norm();
            }
        }

        if (argc == 7){
            mesh = ShortestPathLib::Dijkstra(mesh, stoi(argv[5]), stoi(argv[6]));
        }
        
        

        mesh.ExportTXT();

        UCDUtilities utilities;

        // export nodes
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.ShortestPathNodes)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
        
        

        // // export edges
        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.ShortestPathEdges)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrema,
                                 {},
                                 cell1Ds_properties);
        


        // // export polygon
        if (p == 3 && b != c) utilities.ExportPolygons("./Cell2Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell2DsVertices);


    }else{
        cerr << "Invalid input. Input arguments must be either 4 or 6." << endl;
        return 1;
    }
   
    

    

    
    return 0;
}