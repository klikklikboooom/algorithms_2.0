#include<iostream>
#include<vector>
#include<queue>
#include<cmath>

using namespace std;

struct neighbors{
    int node, weight;
};

struct lowerBoundNeighbors {
    int node; 
    pair<int,int> weight;
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
                residualGraph[i].push_back({parentNode, thisBottleneck});

        }
        maxFlow += thisBottleneck;
    }

    // for(int i =0;i<residualGraph.size();i++) {
    //     for(int j =0;j<residualGraph[i].size();j++) {
    //         cout<<residualGraph[i][j].node<<","<<residualGraph[i][j].weight<<" , ";
    //     }
    //     cout<<endl;
    // }

    return maxFlow;
}

bool checkCirculation(vector<pair<int,vector<neighbors>>> graph) {
    int sum = 0;
    for(int i =0;i<graph.size();i++)
        sum += graph[i].first;
    
    if(sum != 0)
        return false;
    // for(int i =0;i<graph.size();i++) {
    //     int outSum =0, inSum = 0;
    //     for(int j = 0; j<graph[i].second.size();j++)
    //         outSum += graph[i].second[j].weight;

    //     for(int j =1;j<graph.size();j++) {
    //         for(int k =0;k<graph[(i+j)%graph.size()].second.size();k++) {
    //             if(graph[(i+j)%graph.size()].second[k].node == i)
    //                 inSum +=
    //         }
    //     } 
    // }
    return true;
}

vector<vector<neighbors>> reconstructGraph(vector<pair<int,vector<neighbors>>> graph) {
     
    vector<vector<neighbors>> modifiedCirculationGraph(graph.size() + 2);
    int sink = modifiedCirculationGraph.size()-1;
    for(int i =0; i< graph.size();i++) {
        if(graph[i].first < 0)
            modifiedCirculationGraph[0].push_back({i+1, -(graph[i].first)});
        else if (graph[i].first > 0) 
            modifiedCirculationGraph[i+1].push_back({sink, graph[i].first});
        for(int j = 0;j<graph[i].second.size();j++)
            modifiedCirculationGraph[i+1].push_back({graph[i].second[j].node+1, graph[i].second[j].weight});
    }
    // for(int i =0;i<modifiedCirculationGraph.size();i++) {
    //     for(int j = 0; j< modifiedCirculationGraph[i].size();j++) {
    //         cout<<modifiedCirculationGraph[i][j].node<<","<<modifiedCirculationGraph[i][j].weight<<" , ";
    //     }
    //     cout<<endl;
    // }
    return modifiedCirculationGraph;
}

vector<pair<int,vector<neighbors>>> convertIntoCirculationWithDemandsGraph(vector<pair<int,vector<lowerBoundNeighbors>>> graph) {
    vector<pair<int,vector<neighbors>>> circulationWithDemandsGraph(graph.size());
    for(int i =0;i<graph.size();i++) {
        int newDemandForNode;
        circulationWithDemandsGraph[i].first = graph[i].first;

        for(int j = 0; j<graph[i].second.size();j++) {
            circulationWithDemandsGraph[i].first += graph[i].second[j].weight.first;
            circulationWithDemandsGraph[i].second.push_back({graph[i].second[j].node, graph[i].second[j].weight.second - graph[i].second[j].weight.first});
        }

        for(int j =1;j<graph.size();j++)
            for(int k =0;k<graph[(i+j)%graph.size()].second.size();k++)
                if(graph[(i+j)%graph.size()].second[k].node == i) 
                    circulationWithDemandsGraph[i].first -= graph[(i+j)%graph.size()].second[k].weight.first;
            
    }
    return circulationWithDemandsGraph;
}   

int main() {

    vector<pair<int,vector<lowerBoundNeighbors>>> graph = {
        {-3, {{2, {0,4}}}},
        {-4, {{0, {0,5}},{2, {1,5}},{3, {0,4}}}},
        {2,  {{3, {0,3}}}},
        {5, {}},
    };

    // vector<pair<int,vector<neighbors>>> graph = {
    //     {-8, {{3, 6}, {4,7}}},
    //     {-6, {{3,7}, {5,9}}},
    //     {-7, {{0,10}, {3,3}}},
    //     {10, {}},
    //     {0,  {{1,4},{5,4}}},
    //     {11, {}}
    // };

    vector<pair<int,vector<neighbors>>> circulationWithDemandsGraph = convertIntoCirculationWithDemandsGraph(graph);
    for(int i =0;i<circulationWithDemandsGraph.size();i++) {
        cout<<circulationWithDemandsGraph[i].first<<endl;
        for(int j=0;j<circulationWithDemandsGraph[i].second.size();j++)
            cout<<circulationWithDemandsGraph[i].second[j].node<<","<<circulationWithDemandsGraph[i].second[j].weight<<" , ";
        cout<<endl;
    }
        

    if(checkCirculation(circulationWithDemandsGraph)) {
        vector<vector<neighbors>> reconstructedGraph = reconstructGraph(circulationWithDemandsGraph);
        cout << "The maximum possible circulation is "
            << fordFulkersons(reconstructedGraph, 0, reconstructedGraph.size()-1)<<endl;
    } else
        cout<<"Circulation not feasible"<<endl;
    return 0;
}