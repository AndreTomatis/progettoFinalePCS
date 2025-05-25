#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh, string filename)
{

    if(!ImportCell0Ds(mesh, filename+"Cells0D.csv")){
        return false;
    }
        

    if(!ImportCell1Ds(mesh, filename+"Cells1D.csv")){
        return false;
    }

    if(!ImportCell2Ds(mesh, filename+"Cells2D.csv")){
        return false;
    }
    
    if(!ImportCell3Ds(mesh, filename+"Cells3D.csv")){
        return false;
    }

    return true;

}
// ***************************************************************************


bool ImportCell0Ds(PolygonalMesh& mesh, string filename)
{
    ifstream file(filename);

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        char delimiter;

        converter >>  id >> delimiter >> mesh.Cell0DsCoordinates(0, id) >> delimiter >> mesh.Cell0DsCoordinates(1, id) >> delimiter >> mesh.Cell0DsCoordinates(2, id);

        mesh.Cell0DsId.push_back(id);

    }

    return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh, string filename)
{
    ifstream file(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        char delimiter;

        converter >> id >>  delimiter >> mesh.Cell1DsExtrema(0, id) >> delimiter >>  mesh.Cell1DsExtrema(1, id);
        mesh.Cell1DsId.push_back(id);
    }

    return true;
}
// ***************************************************************************
vector<string> SplitCSVLine(const string& line) {
    vector<string> tokens;
    string token;
    istringstream stream(line);
    while (getline(stream, token, ';')) {
        tokens.push_back(token);
    }
    return tokens;
}


bool ImportCell2Ds(PolygonalMesh& mesh, string filename)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    
    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);
    
    for (const string& line : listLines)
    {
        
        vector<string> tokens = SplitCSVLine(line);
        size_t idx = 0;

        if (tokens.size() < 6) // id, nVertices, at least 1 vertex, nEdges, at least 1 edge
        {
            std::cerr << "Malformed line in Cell2Ds.csv: " << line << std::endl;
        }

        unsigned int id = stoi(tokens[idx++]);

        unsigned int nVertices = stoi(tokens[idx++]);
        vector<unsigned int> vertices(nVertices);

        
        for (unsigned int i = 0; i < nVertices; ++i)
            vertices[i] = stoi(tokens[idx++]);

        unsigned int nEdges = stoi(tokens[idx++]);
        vector<unsigned int> edges(nEdges);
        for (unsigned int i = 0; i < nEdges; ++i)
            edges[i] = stoi(tokens[idx++]);
        
        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);      
    }

    return true;
}

bool ImportCell3Ds(PolygonalMesh& mesh, string filename)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell3Ds = listLines.size();

    if (mesh.NumCell3Ds == 0)
    {
        cerr << "There is no cell 3D" << endl;
        return false;
    }

    mesh.Cell3DsId.reserve(mesh.NumCell3Ds);
    mesh.Cell3DsVertices.reserve(mesh.NumCell3Ds);
    mesh.Cell3DsEdges.reserve(mesh.NumCell3Ds);
    mesh.Cell3DsFaces.reserve(mesh.NumCell3Ds);
    
    for (const string& line : listLines)
    {
        
        vector<string> tokens = SplitCSVLine(line);
        size_t idx = 0;

        if (tokens.size() < 6) // id, nVertices, at least 1 vertex, nEdges, at least 1 edge
        {
            std::cerr << "Malformed line in file: " << line << std::endl;
        }

        unsigned int id = stoi(tokens[idx++]);
        unsigned int nVertices = stoi(tokens[idx++]);
        vector<unsigned int> vertices(nVertices);

        
        for (unsigned int i = 0; i < nVertices; ++i)
            vertices[i] = stoi(tokens[idx++]);

        unsigned int nEdges = stoi(tokens[idx++]);
        vector<unsigned int> edges(nEdges);
        for (unsigned int i = 0; i < nEdges; ++i)
            edges[i] = stoi(tokens[idx++]);

        unsigned int nFaces = stoi(tokens[idx++]);
        vector<unsigned int> faces(nFaces);
        for (unsigned int i = 0; i < nFaces; ++i)
            faces[i] = stoi(tokens[idx++]);
        
        mesh.Cell3DsId.push_back(id);
        mesh.Cell3DsVertices.push_back(vertices);
        mesh.Cell3DsEdges.push_back(edges);
        mesh.Cell3DsFaces.push_back(faces);
    }

    return true;
}


bool check_arguments(unsigned int p, unsigned int q, unsigned int b, unsigned int c){
    return (p <= 5 && p >= 3 &&
            q <= 5 && q >= 3 &&
            ((b >= 1 && c == 0) || (b == 0 && c >= 1)) &&
            (p == 3 || q == 3)
            );
}


PolygonalMesh Triangulation_1(PolygonalMesh mesh, unsigned int b){

    PolygonalMesh geodetic(mesh.p, mesh.q);
    Point ps[3];

    int T = b*b;
    geodetic.NumCell0Ds = 2*T + 2;

    for(size_t i = 0; i < mesh.Cell2DsVertices.size(); ++i){
        for(int j = 0; j < mesh.Cell2DsVertices[i].size(); ++j){
            ps[j].x = mesh.Cell0DsCoordinates(0, mesh.Cell2DsVertices[i][j]);
            ps[j].y = mesh.Cell0DsCoordinates(1, mesh.Cell2DsVertices[i][j]);
            ps[j].z = mesh.Cell0DsCoordinates(2, mesh.Cell2DsVertices[i][j]);
        }
    
        Point u = (ps[1] - ps[0]) * (1.0 / b); // Vettore da A verso B
        Point v = (ps[2] - ps[0]) * (1.0 / b); // Vettore da A verso C
        
        for (int h = 0; h < b + 1; ++h) {
            Point p = ps[0] + u * h; 
            for (int j = 0; j < b - h + 1; ++j) {
                p = p + v;
                //store points and vertices
            }
        }
        
    }


    return geodetic;
}


}
