#include<iostream>
#include<vector>
#include<map>
#include<queue>
#define INF numeric_limits<int>::infinity()

using namespace std;

struct neighbors{
    int node, weight;
};

int parent[6] = {-2, -2, -2, -2, -2, -2};
bool bfs(vector<vector<neighbors>> residualGraph, int s, int t) {
    vector<bool> visited(residualGraph.size(), false);

    queue<int> q;

    q.push(s);
    visited[s] = 0;
    parent[s] = -1;

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int i =0;i<residualGraph[u].size();i++) {
            neighbors otherNode = residualGraph[u][i];
            if(visited[otherNode.node] == false && otherNode.weight > 0) {
                parent[otherNode.node] = u;
                if(otherNode.node == t)
                    return true;

                q.push(otherNode.node);
                visited[otherNode.node] = true;
            }
        }
    }

    return false;
}

int bottleneck(int s, int t, vector<vector<neighbors>> residualGraph) {
    int min_residual_capacity = INT_MAX;
    for(int i = t; i !=s; i = parent[i]) {
        int thisPathFlow;
        int parentNode = parent[i];
        for(int j =0; j< residualGraph[parentNode].size();j++) {
            if(residualGraph[parentNode][j].node == i) {
                thisPathFlow = residualGraph[parentNode][j].weight;
            }
        }
        min_residual_capacity = min(min_residual_capacity, thisPathFlow);
    }
    return min_residual_capacity;
}

int fordFulkersons(vector<vector<neighbors>> graph, int s, int t) {
    //Set the residual graph to have the same values as the main graph
    vector<vector<neighbors>> residualGraph = graph;

    int maxFlow = 0;
    while(bfs(residualGraph,s,t)) {
        //Calculate the bottleneck. The bottleneck is the minimum 
        //residual capacity on the s-t path
        int thisBottleneck = bottleneck(s,t, residualGraph);
        bool backwardPathExists = false;
        //For each edge on the s-t path, Subtract the bottleneck in the forward 
        //direction and add the bottleneck in the reverse direction of 
        //the residual graph. If the reverse edge doesn't exist, create it.
        for(int i = t; i != s; i = parent[i]) {
            int parentNode = parent[i];
            for(int j = 0;j < residualGraph[parentNode].size(); j++) {
                if(residualGraph[parentNode][j].node == i)
                    residualGraph[parentNode][j].weight -= thisBottleneck; 
            }

            for(int j = 0;j < residualGraph[i].size(); j++) {
                if(residualGraph[i][j].node == i) {
                    residualGraph[i][j].weight += thisBottleneck; 
                    backwardPathExists = true;
                }
            }

            if(!backwardPathExists)
                residualGraph[i].push_back({parentNode, -(thisBottleneck)});

        }
        maxFlow += thisBottleneck;
    }

    return maxFlow;
}


int main() {

    vector<vector<neighbors>> graph = {
        {{1, 16}, {2,13}},
        {{2,10}, {3,12}},
        {{1,4}, {4,14}},
        {{2,9}, {5,20}},
        {{3,7}, {5,4}},
        {}
    };

    cout << "The maximum possible flow is "
         << fordFulkersons(graph, 0, 5)<<endl;
    return 0;
}