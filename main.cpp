#include <iostream>
#include "Polygon.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;

int main(int argc, char* argv[])
{
    

    string filename = "./Polygons/";
    
    unsigned int p,q,b,c;

    if (argc == 5 || argc == 7){
        
        p = stoi(argv[1]); q = stoi(argv[2]); b = stoi(argv[3]); c = stoi(argv[4]);
        
        if (!check_arguments(p,q,b,c)){
            cerr << "Invalid values of p, q, b or c" << endl;
            return 1;
        }

        PolygonalMesh mesh(p,q);

        if (q == 3)
            filename += "./p" + to_string(p) + "q" + to_string(q) + "/";

        if (p == 3 && q != 3)
            filename += "./p" + to_string(q) + "q" + to_string(p) + "/";

        if(!ImportMesh(mesh, filename))
        {
            cerr << "Unable to import the mesh, something went wrong" << endl;
            return 1;
        }

        if (p==3 && q != 3) mesh = mesh.CreateDual();

        mesh.ExportTXT();

    }else{
        cerr << "Invalid input. Input arguments must be either 4 or 6." << endl;
        return 1;
    }
   
    

    

    /*UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp",
                           mesh.Cell0DsCoordinates);

    utilities.ExportSegments("./Cell1Ds.inp",
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema);
    */
    return 0;
}