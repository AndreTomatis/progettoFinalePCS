using namespace std;
using namespace Eigen;


PolygonalMesh CreateDual(const PolygonalMesh& mesh){

    PolygonalMesh dual;
    
    dual.NumCell0Ds = mesh.NumCell2Ds;
    dual.Cell0DsId.resize(dual.NumCell0Ds);
    dual.Cell0DsCoordinates = MatrixXd::Zero(3, dual.NumCell0Ds);


    // new vertices as centres of old faces

    for (unsigned int f_id = 0; f_id < mesh.NumCell2Ds; ++f_id) {

        dual.Cell0DsId[f_id] = f_id;

        Vector3d centre = Vector3d::Zero();

        for (unsigned int v_id : mesh.Cell2DsVertices[f_id]) {
            centre += mesh.Cell0DsCoordinates.col(v_id);
        }

        centre /= mesh.Cell2DsVertices[f_id].size();

        dual.Cell0DsCoordinates.col(f_id) = centre;

    }



    // pairs old edges to their adjacent faces

    map<pair<unsigned int, unsigned int>, vector<unsigned int>> edge_to_faces; // key is a pair of vertex Ids defining each edge, with vector of face Ids adjacent to that edge

    for (unsigned int f_id = 0; f_id < mesh.NumCell2Ds; ++f_id) {

        const auto& edge_ids = mesh.Cell2DsEdges[f_id];
        const auto& vertex_ids = mesh.Cell2DsVertices[f_id];

        for (size_t i = 0; i < edge_ids.size(); ++i) {

            unsigned int e_id = edge_ids[i];

            auto v0 = mesh.Cell1DsExtrema(0, e_id);
            auto v1 = mesh.Cell1DsExtrema(1, e_id);

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

            edge_pairs.emplace_back(f1, f2);

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

    dual.NumCell2Ds = mesh.NumCell0Ds;
    dual.Cell2DsId.resize(dual.NumCell2Ds);
    dual.Cell2DsVertices.resize(dual.NumCell2Ds);
    dual.Cell2DsEdges.resize(dual.NumCell2Ds);

    for (unsigned int v_id = 0; v_id < mesh.NumCell0Ds; ++v_id) {
        vector<unsigned int> incident_faces;

        // looks for v_id we're working on in each face
        for (unsigned int f_id = 0; f_id < mesh.NumCell2Ds; ++f_id) { 
            const auto& verts = mesh.Cell2DsVertices[f_id];

            if (find(verts.begin(), verts.end(), v_id) != verts.end()) 
            {
                incident_faces.push_back(f_id);
            }
        }

        vector<unsigned int> face_ids = incident_faces;
        vector<unsigned int> edge_ids;

        for (size_t i = 0; i < face_ids.size(); ++i) {
            unsigned int f1 = face_ids[i];
            unsigned int f2 = face_ids[(i + 1) % face_ids.size()];
            edge_ids.push_back(edge_index_map[{f1, f2}]);
        }

        dual.Cell2DsId[v_id] = v_id;
        dual.Cell2DsVertices[v_id] = face_ids;
        dual.Cell2DsEdges[v_id] = edge_ids;
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


    return dual;
}





