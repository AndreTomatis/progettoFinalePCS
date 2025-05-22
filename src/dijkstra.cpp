#include <iostream>
#include <fstream>
#include "Eigen/Eigen"
#include <vector>
#include <set>
#include <climits>
#include <queue>
#include <algorithm>

using namespace std;
using namespace Eigen;


// construct adjacency list from our mesh
vector<vector<unsigned int>> AdjList(Eigen::MatrixXi& edges, unsigned int V){
    
    vector<vector<unsigned int>> adj(V); 
    for (int i = 0; i < edges.rows(); ++i){

        unsigned int u = edges(i,0);
        unsigned int v = edges(i,1);
        
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected

    }
    return adj;
}



// colours a given path on our mesh by markers
PolygonalMesh ColourPath(PolygonalMesh &mesh, const vector<pair<unsigned int, unsigned int>> &path){

    PolygonalMesh ColouredMesh = mesh;

    return ColouredMesh;
}



// Dijkstra's algorithm on our mesh
PolygonalMesh Dijkstra(const PolygonalMesh& mesh, const unsigned int& src, const unsigned int& dest){

    unsigned int V = mesh.NumCell1Ds;

    vector<vector<unsigned int>> adj = AdjList(mesh.Cell1DsExtrema, mesh.NumCell1Ds);

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


    // remember shortest path
    vector<pair<unsigned int, unsigned int>> path;
    if (dist[dest] == INT_MAX){
        return mesh; // No path
    }

    for (unsigned int cur = dest; pred[cur] != -1; cur = pred[cur]){
        path.push_back({pred[cur], cur});
    }
    reverse(path.begin(), path.end());


    PolygonalMesh mesh = ColourPath(path);
    return mesh;
}
