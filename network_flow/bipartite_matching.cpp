#include<iostream>
#include<vector>
#include<queue>
#include<cmath>

using namespace std;

struct neighbors{
    int node, weight;
};

int parent[6] = {-2, -2, -2, -2, -2, -2};

int nextPowerOf2(int n) {
    int count = 0;
    int copyOfN = n;
    while(copyOfN != 0) {
        copyOfN/=2;
        count++;
    }

    return pow(2, count-1);
}

vector<vector<neighbors>> reconstructGraph(vector<vector<neighbors>> residualGraph, int threshold) {
    vector<vector<neighbors>> copyOfResidualGraph = residualGraph;
    for(int i =0;i<copyOfResidualGraph.size();i++)
        for(int j =0;j<copyOfResidualGraph[i].size();j++) 
            if(copyOfResidualGraph[i][j].weight < threshold)
                copyOfResidualGraph[i].erase(copyOfResidualGraph[i].begin()+j);

    return copyOfResidualGraph;
}



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




int scalingMaxFlow(vector<vector<neighbors>> graph, int s, int t) {
    //Set the residual graph to have the same values as the main graph
    vector<vector<neighbors>> residualGraph = graph;
    int maxFlow = 0;

    int maxCapOutOfS = -1;
    for(int i =0;i<graph[s].size();i++)
        maxCapOutOfS = max(maxCapOutOfS, graph[s][i].weight);

    int delta = nextPowerOf2(maxCapOutOfS);

    while(delta >= 1) {
        vector<vector<neighbors>> deltaResidualGraph = reconstructGraph(residualGraph, delta);
        while(bfs(deltaResidualGraph,s,t)) {
            //Calculate the bottleneck. The bottleneck is the minimum 
            //residual capacity on the s-t path
            int thisBottleneck = bottleneck(s,t, deltaResidualGraph);
            bool backwardPathExistsInDeltaResidual = false;
            bool backwardPathExistsInResidual = false;
            //For each edge on the s-t path, Subtract the bottleneck in the forward 
            //direction and add the bottleneck in the reverse direction of 
            //the residual graph. If the reverse edge doesn't exist, create it.
            for(int i = t; i != s; i = parent[i]) {
                int parentNode = parent[i];
                for(int j = 0;j < deltaResidualGraph[parentNode].size(); j++) {
                    if(deltaResidualGraph[parentNode][j].node == i) {
                        deltaResidualGraph[parentNode][j].weight -= thisBottleneck;
                        for(int k =0;k<residualGraph[parentNode].size();k++)
                            if(residualGraph[parentNode][k].node == deltaResidualGraph[parentNode][j].node)
                                residualGraph[parentNode][k].weight -= thisBottleneck;
                    }
                }

                for(int j = 0;j < deltaResidualGraph[i].size(); j++) {
                    if(deltaResidualGraph[i][j].node == i) {
                        deltaResidualGraph[i][j].weight += thisBottleneck;
                        for(int k =0;k<residualGraph[parentNode].size();k++) {
                            if(residualGraph[parentNode][k].node == deltaResidualGraph[parentNode][j].node) {
                                residualGraph[i][j].weight += thisBottleneck; 
                                backwardPathExistsInResidual = true;
                            }
                        }    
                    }
                }

                if(!backwardPathExistsInResidual)
                    residualGraph[i].push_back({parentNode, thisBottleneck});

                if(!backwardPathExistsInDeltaResidual)
                    deltaResidualGraph[i].push_back({parentNode,thisBottleneck});

            }
            maxFlow += thisBottleneck;
        }
        delta /= 2;
    }

    return maxFlow;
    
}

int findMaxMatching(vector<vector<neighbors>> bipartiteGraph, vector<int> X, vector<int> Y) {
    vector<vector<neighbors>> maxFlowGraph(X.size() + Y.size() + 2);
    //Since the source of the new graph is 0, we need to increment all the nodes by 1
    for(int i =0;i<X.size();i++) {
        maxFlowGraph[0].push_back({X[i]+1, 1});
        for(int j =0;j<bipartiteGraph[X[i]].size();j++) {
            maxFlowGraph[X[i]+1].push_back({bipartiteGraph[X[i]][j].node+1, bipartiteGraph[X[i]][j].weight});
        }
    }


    int sink = X.size() + Y.size() + 1;

    for(int i =0; i<Y.size();i++)
        maxFlowGraph[Y[i]+1].push_back({sink, 1});

    int maxFlow = scalingMaxFlow(maxFlowGraph, 0, sink);
    return maxFlow;
}


int main() {

    vector<vector<neighbors>> bipartiteInputGraph = {
        {{7,1}},
        {{6,1},{5,1}},
        {{6,1},{7,1}},
        {{6,1},{7,1}},
        {{7,1}, {8,1}, {9,1}},
        {{1,1}},
        {{1,1}, {2,1}, {3,1}},
        {{2,1}, {3,1}, {4,1}},
        {{4,1}},
        {{4,1}}
    };

    vector<int> X = {0,1,2,3,4};
    vector<int> Y = {5,6,7,8,9};

    cout << "The maximum possible fmatching is "
         << findMaxMatching(bipartiteInputGraph, X, Y)<<endl;
    return 0;
}