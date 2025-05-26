#pragma once

#include <iostream>
#include <fstream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int p,q;

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

    PolygonalMesh(unsigned int p, unsigned int q) : p(p), q(q) {}


    PolygonalMesh CreateDual(){

        PolygonalMesh dual(q,p);
        
        dual.NumCell0Ds = NumCell2Ds;
        dual.Cell0DsId.resize(dual.NumCell0Ds);
        dual.Cell0DsCoordinates = MatrixXd::Zero(3, dual.NumCell0Ds);


        // new vertices as centres of old faces

        for (unsigned int f_id = 0; f_id < NumCell2Ds; ++f_id) {

            dual.Cell0DsId[f_id] = f_id;

            Vector3d centre = Vector3d::Zero();

            for (unsigned int v_id : Cell2DsVertices[f_id]) {
                centre += Cell0DsCoordinates.col(v_id);
            }

            centre /= Cell2DsVertices[f_id].size();

            dual.Cell0DsCoordinates.col(f_id) = centre;

        }



        // pairs old edges to their adjacent faces

        map<pair<unsigned int, unsigned int>, vector<unsigned int>> edge_to_faces; // key is a pair of vertex Ids defining each edge, with vector of face Ids adjacent to that edge

        for (unsigned int f_id = 0; f_id < NumCell2Ds; ++f_id) {

            const auto& edge_ids = Cell2DsEdges[f_id];
            const auto& vertex_ids = Cell2DsVertices[f_id];

            for (size_t i = 0; i < edge_ids.size(); ++i) {

                unsigned int e_id = edge_ids[i];

                auto v0 = Cell1DsExtrema(0, e_id);
                auto v1 = Cell1DsExtrema(1, e_id);

                auto key = minmax(v0, v1); // reorders in consistent (min, max) order

                edge_to_faces[key].push_back(f_id);
            }
        }



        // create new edges between adjacent old faces' centres

        map<pair<unsigned int, unsigned int>, unsigned int> edge_index_map; // key is a pair of face Id, with Id of connecting dual edge
        vector<pair<unsigned int, unsigned int>> edge_pairs; // pairs of all adjacent faces, ie new dual edge 

        unsigned int dual_edge_id = 0;

        for (const auto& ef : edge_to_faces) {

            const auto& faces = ef.second;

            if (faces.size() == 2) {

                unsigned int f1 = faces[0];
                unsigned int f2 = faces[1];

                edge_pairs.emplace_back(f1, f2); // push_back?

                edge_index_map[{f1, f2}] = dual_edge_id;
                edge_index_map[{f2, f1}] = dual_edge_id;

                dual_edge_id++;
            }
        }

        dual.NumCell1Ds = edge_pairs.size();
        dual.Cell1DsId.resize(dual.NumCell1Ds);
        dual.Cell1DsExtrema = MatrixXi(2, dual.NumCell1Ds);

        for (unsigned int i = 0; i < edge_pairs.size(); ++i) {
            dual.Cell1DsId[i] = i;
            dual.Cell1DsExtrema(0, i) = edge_pairs[i].first;
            dual.Cell1DsExtrema(1, i) = edge_pairs[i].second;
        }

        // create new faces

        dual.NumCell2Ds = NumCell0Ds;
        dual.Cell2DsId.resize(dual.NumCell2Ds);
        dual.Cell2DsVertices.resize(NumCell2Ds);
        dual.Cell2DsEdges.resize(q*NumCell2Ds/2);

        for (unsigned int v_id = 0; v_id < NumCell0Ds; ++v_id) {
            vector<unsigned int> incident_faces;

            // looks for v_id we're working on in each face
            for (unsigned int f_id = 0; f_id < NumCell2Ds; ++f_id) { 
                const auto& verts = Cell2DsVertices[f_id];

                if (find(verts.begin(), verts.end(), v_id) != verts.end()) 
                {
                    incident_faces.push_back(f_id);
                }
            }

            vector<unsigned int> face_ids = incident_faces;
            vector<unsigned int> edge_ids;

            for (size_t i = 0; i < q; ++i) {
                unsigned int f1 = face_ids[i];
                unsigned int f2 = face_ids[(i + 1) % q];
                edge_ids.push_back(edge_index_map[{f1, f2}]);
            }

            dual.Cell2DsId[v_id] = v_id;
            dual.Cell2DsVertices.push_back(face_ids);
            dual.Cell2DsEdges.push_back(edge_ids);
        }



        // store last few things in Cell3Ds

        dual.NumCell3Ds = 1;
        dual.Cell3DsId = {0};
        dual.Cell3DsVertices = {dual.Cell0DsId}; // all dual vertices
        dual.Cell3DsEdges = {dual.Cell1DsId};    // all dual edges
        dual.Cell3DsFaces.resize(1);

        for (unsigned int i = 0; i < dual.NumCell2Ds; ++i){
            dual.Cell3DsFaces[0].push_back(i);
        }

        // project the vertices on a circumference
        for(unsigned int i = 0; i < dual.NumCell0Ds; i++){
            dual.Cell0DsCoordinates.col(i) /= dual.Cell0DsCoordinates.col(i).norm();
        }


        return dual;
    }



    void ExportTXT(){

        ofstream ofs("./Cell0Ds.txt");

        if (!ofs.is_open()) {
            cerr << "Can't open " << std::endl;
            return;
        }

        ofs << "Id;X;Y;Z" << endl;

        for(unsigned int k=0; k < Cell0DsId.size(); k++)
            ofs << Cell0DsId[k] << ";" << Cell0DsCoordinates(0,k) << ";" << Cell0DsCoordinates(1,k) << ";" << Cell0DsCoordinates(2,k) << endl;
        ofs.close();

        ofstream ofs2("./Cell1Ds.txt");

        if (!ofs2.is_open()) {
            cerr << "Can't open " << std::endl;
            return;
        }

        ofs2 << "Id;Origin;End" << endl;

        for(unsigned int k=0; k < Cell1DsId.size(); k++)
            ofs2 << Cell1DsId[k] << ";" << Cell1DsExtrema(0,k) << ";" << Cell1DsExtrema(1,k) << endl;
        ofs2.close();


        ofstream ofs3("./Cell2Ds.txt");

        if (!ofs3.is_open()) {
            cerr << "Can't open " << std::endl;
            return;
        }

        ofs3 << "Id;NumVertices;Vertices;NumEdges;Edges" << endl;

        for(unsigned int k=0; k < Cell2DsId.size(); k++){
            ofs3 << Cell2DsId[k] << ";"
                 << Cell2DsVertices[k].size() << ";";
            
            for (unsigned int j = 0; j < Cell2DsVertices[k].size(); j++)
                ofs3 << Cell2DsVertices[k][j] << ";";
            
            ofs3 << Cell2DsEdges[k].size() << ";";
            for (unsigned int j = 0; j < Cell2DsEdges[k].size(); j++)
                ofs3 << Cell2DsEdges[k][j] << ";";
            
            ofs3 << endl;
        }
        ofs3.close();


        ofstream ofs4("./Cell3Ds.txt");

        if (!ofs4.is_open()) {
            cerr << "Can't open " << std::endl;
            return;
        }

        ofs4 << "Id;NVertices;Vertices;NEdges;Edges;NFaces;Faces" << endl;

        for(unsigned int k=0; k < Cell3DsId.size(); k++){
            ofs4 << Cell3DsId[k] << ";"
                 << Cell3DsVertices[k].size() << ";";
            
            for (unsigned int j = 0; j < Cell3DsVertices[k].size(); j++)
                ofs4 << Cell3DsVertices[k][j] << ";";
            
            ofs4 << Cell3DsEdges[k].size() << ";";
            for (unsigned int j = 0; j < Cell3DsEdges[k].size(); j++)
                ofs4 << Cell3DsEdges[k][j] << ";";
            
            ofs4 << Cell3DsFaces[k].size() << ";";
            for (unsigned int j = 0; j < Cell3DsFaces[k].size(); j++)
                ofs4 << Cell3DsFaces[k][j] << ";";
            
            ofs4 << endl;
        }
        ofs4.close();
        
        return;
    }

};

}