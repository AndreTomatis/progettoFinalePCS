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
        cout << "0D";
        return false;
    }
        

    if(!ImportCell1Ds(mesh, filename+"Cells1D.csv")){
        cout << "1D";
        return false;
    }

    if(!ImportCell2Ds(mesh, filename+"Cells2D.csv")){
        cout << "2D";
        return false;
    }
    
    if(!ImportCell3Ds(mesh, filename+"Cells3D.csv")){
        cout << "3D";
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
        
        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
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


}
