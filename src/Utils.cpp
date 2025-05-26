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


PolygonalMesh Triangulation_1(PolygonalMesh mesh, unsigned int b, unsigned int T){

    PolygonalMesh geodetic(mesh.p, mesh.q);
    Point ps[3];

    switch (mesh.q)
    {
    case 3:
        geodetic.NumCell0Ds = 2*T + 2;
        geodetic.NumCell1Ds = 6*T;
        geodetic.NumCell2Ds = 4*T;
        break;
    case 4:
        geodetic.NumCell0Ds = 4*T + 2;
        geodetic.NumCell1Ds = 12*T;
        geodetic.NumCell2Ds = 8*T;
        break;
    case 5:
        geodetic.NumCell0Ds = 10*T + 2;
        geodetic.NumCell1Ds = 30*T;
        geodetic.NumCell2Ds = 20*T;
        break;
    }

    
    geodetic.Cell0DsId.reserve(geodetic.NumCell0Ds);
    geodetic.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, geodetic.NumCell0Ds);
    
    
    geodetic.Cell1DsId.reserve(geodetic.NumCell1Ds);
    geodetic.Cell1DsExtrema = Eigen::MatrixXi(2, geodetic.NumCell1Ds);
    
    
    geodetic.Cell2DsId.reserve(geodetic.NumCell2Ds);
    geodetic.Cell2DsVertices.resize(geodetic.NumCell2Ds);
    geodetic.Cell2DsEdges.reserve(geodetic.NumCell2Ds);

    unsigned int id_cnt = 0;
    unsigned int edge_cnt = 0;
    unsigned int face_cnt = 0;
    vector<vector<unsigned int>> old_ps(b+1);

    for(size_t i = 0; i < mesh.Cell2DsVertices.size(); ++i){

        for(int j = 0; j < mesh.Cell2DsVertices[i].size(); ++j){
            ps[j].x = mesh.Cell0DsCoordinates(0, mesh.Cell2DsVertices[i][j]);
            ps[j].y = mesh.Cell0DsCoordinates(1, mesh.Cell2DsVertices[i][j]);
            ps[j].z = mesh.Cell0DsCoordinates(2, mesh.Cell2DsVertices[i][j]);
        }
        
        Point u = (ps[1] - ps[0]) * (1.0 / b); // Vettore da A verso B
        Point v = (ps[2] - ps[0]) * (1.0 / b); // Vettore da A verso C

        
        
        for (int h = 0; h < b + 1; ++h) {

            
            old_ps[h].resize(b-h+1); 

            for (int j = 0; j < b - h + 1; ++j) {
                cout << face_cnt << " " << edge_cnt << " " << h << " " << j << endl;

                Point p = ps[0] + u*h + v*j; 
                
                //store points and vertices
                int id = get_id(p, geodetic);
                if (id == -1){
                    id = id_cnt++;
                    geodetic.Cell0DsId.push_back(id);
                    geodetic.Cell0DsCoordinates(0,id) = p.x;
                    geodetic.Cell0DsCoordinates(1,id) = p.y;
                    geodetic.Cell0DsCoordinates(2,id) = p.z;
                }
                
                if (h > 0){
                    int edge_id = get_edge(id, old_ps[h-1][j], geodetic);
                    if (edge_id == -1){
                        geodetic.Cell1DsId.push_back(edge_cnt);
                        geodetic.Cell1DsExtrema(0,edge_cnt) = id;
                        geodetic.Cell1DsExtrema(1,edge_cnt) = old_ps[h-1][j];
                        edge_cnt++;
                    }

                    edge_id = get_edge(id, old_ps[h-1][j+1], geodetic);
                    if (edge_id == -1){
                        geodetic.Cell1DsId.push_back(edge_cnt);
                        geodetic.Cell1DsExtrema(0,edge_cnt) = id;
                        geodetic.Cell1DsExtrema(1,edge_cnt) = old_ps[h-1][j+1];
                        edge_cnt++;
                    }

                    geodetic.Cell2DsId.push_back(face_cnt);
                    geodetic.Cell2DsVertices[face_cnt].resize(3);
                    geodetic.Cell2DsVertices[face_cnt][0] = id;
                    geodetic.Cell2DsVertices[face_cnt][1] = old_ps[h-1][j];
                    geodetic.Cell2DsVertices[face_cnt][2] = old_ps[h-1][j+1];
                    geodetic.Cell2DsEdges[face_cnt].reserve(3);
                    geodetic.Cell2DsEdges[face_cnt].push_back(get_edge(id, old_ps[h-1][j+1], geodetic));
                    geodetic.Cell2DsEdges[face_cnt].push_back(get_edge(id, old_ps[h-1][j], geodetic));
                    geodetic.Cell2DsEdges[face_cnt].push_back(get_edge(old_ps[h-1][j], old_ps[h-1][j+1], geodetic));
                    face_cnt++;
                }

                if (j>0){
                    int edge_id = get_edge(id, old_ps[h][j-1], geodetic);
                    if (edge_id == -1){
                        geodetic.Cell1DsId.push_back(edge_cnt);
                        geodetic.Cell1DsExtrema(0,edge_cnt) = id;
                        geodetic.Cell1DsExtrema(1,edge_cnt) = old_ps[h][j-1];
                        edge_cnt++;
                    }

                    if (h > 0){
                        geodetic.Cell2DsId.push_back(face_cnt);
                        geodetic.Cell2DsVertices[face_cnt].resize(3);
                        geodetic.Cell2DsVertices[face_cnt][0] = id;
                        geodetic.Cell2DsVertices[face_cnt][1] = old_ps[h][j-1];
                        geodetic.Cell2DsVertices[face_cnt][2] = old_ps[h-1][j];
                        geodetic.Cell2DsEdges[face_cnt].reserve(3);
                        geodetic.Cell2DsEdges[face_cnt].push_back(get_edge(id, old_ps[h][j-1], geodetic));
                        geodetic.Cell2DsEdges[face_cnt].push_back(get_edge(id, old_ps[h-1][j], geodetic));
                        geodetic.Cell2DsEdges[face_cnt].push_back(get_edge(old_ps[h-1][j], old_ps[h][j-1], geodetic));
                        face_cnt++;
                    }
                    
                }

                old_ps[h][j] = id;
                
                
            }
            
            
        }
        
        
    }


    return geodetic;
}


int get_id(Point p, PolygonalMesh geodetic){
    for (unsigned int i =0; i < geodetic.Cell0DsCoordinates.cols(); i++){
        if (abs(p.x - geodetic.Cell0DsCoordinates(0,i)) <= 0.0001
            && abs(p.y - geodetic.Cell0DsCoordinates(1,i)) <= 0.0001
            && abs(p.z - geodetic.Cell0DsCoordinates(2,i)) <= 0.0001){
            return i;
        }
    }
    return -1;
}


int get_edge(unsigned int id1, unsigned int id2, PolygonalMesh geodetic){
    for (unsigned int i =0; i < geodetic.Cell1DsExtrema.cols(); i++){
        if ((geodetic.Cell1DsExtrema(0,i) == id1 && geodetic.Cell1DsExtrema(1,i) == id2)
            || (geodetic.Cell1DsExtrema(0,i) == id2 && geodetic.Cell1DsExtrema(1,i) == id1)){
            return i;
        }
    }
    return -1;
}

}
