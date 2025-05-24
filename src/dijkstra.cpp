#include <iostream>
#include <fstream>
#include "Eigen/Eigen"
#include "Dijkstra.hpp"
#include <climits>
#include <queue>
#include <algorithm>

namespace ShortestPathLib
{

// construct adjacency list from our mesh
vector<vector<unsigned int>> AdjList(const Eigen::MatrixXi& edges, unsigned int V){
    
    vector<vector<unsigned int>> adj(V); 
    for (int i = 0; i < edges.cols(); ++i){

        unsigned int u = edges(0,i);
        unsigned int v = edges(1,i);
        
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected

    }

    return adj;
}



// colours a given path on our mesh by markers
void ColourPath(PolygonalMesh &mesh, const vector<pair<unsigned int, unsigned int>> &path){

    PolygonalMesh ColouredMesh = mesh;

    return;
}



// Dijkstra's algorithm on our mesh
PolygonalMesh Dijkstra(PolygonalMesh& mesh, const unsigned int& src, const unsigned int& dest){

    unsigned int V = mesh.NumCell1Ds;

    vector<vector<unsigned int>> adj = AdjList(mesh.Cell1DsExtrema, mesh.NumCell0Ds);

    vector<int> dist(V, INT_MAX);
    vector<int> pred(V, -1);
    priority_queue<pair<unsigned int, unsigned int>, vector<pair<unsigned int, unsigned int>>, greater<>> pq; // changes max-heap to min-heap

    dist[src] = 0;

    pq.push({0, src});


    while (!pq.empty()){
        auto [d, u] = pq.top(); pq.pop();

        if (d > dist[u]) continue;

        for (unsigned int v : adj[u]){
            if (dist[v] > dist[u] + 1){
                dist[v] = dist[u] + 1;
                pred[v] = u;
                pq.push({dist[v], v});
            }
        }
    }


    // remember shortest path node pairs
    vector<pair<unsigned int, unsigned int>> path;
    if (dist[dest] == INT_MAX){
        cout << "error" << endl;
        return mesh; // no path was found
    }

    for (unsigned int cur = dest; pred[cur] != -1; cur = pred[cur]){
        path.push_back({pred[cur], cur});
    }
    reverse(path.begin(), path.end());

    cout << "Path:" << endl;
    for (const auto& p : path) {
        cout << "(" << p.first << ", " << p.second << ")" << endl;
    }

    ColourPath(mesh, path);
    return mesh;
}

} //namespace end