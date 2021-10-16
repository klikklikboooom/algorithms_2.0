#include<iostream>
#include<vector>
#include<map>
#define INF numeric_limits<int>::infinity()

using namespace std;

struct neighbors{
    int node, weight;
};

/* Find all the nodes going to a particular node */
vector<neighbors> findOutgoingEdgesNodes(int incomingEdge, vector<vector<neighbors>> adjacencyList) {
    vector<neighbors> outgoingEdgeNodes;
    for(int i =0;i<adjacencyList.size()-1;i++) {
        for(int j =0;j< adjacencyList[j].size();j++) {
            if(adjacencyList[i][j].node == incomingEdge){
                neighbors node = {i, adjacencyList[i][j].weight};
                outgoingEdgeNodes.push_back(node);
            } 
        }
    }

    return outgoingEdgeNodes;
}


vector<int> shortestPath(vector<vector<neighbors>> adjacencyList, int destinationPath) {
    vector<int> memo;
    vector<int> successor;
    vector<bool> changed;
    //Maintain a vector to check if each node has changed in that iteration changed
    for(int i =0;i<adjacencyList.size();i++)
        changed.push_back(true);
    
    for(int i =0;i<adjacencyList.size();i++) {
        memo.push_back(INT_MAX);
        successor.push_back(-1);    
    }

    //Since we're setting the destination node to the last node in the adjacencyList, 
    //set it's memo value to 0
    memo[adjacencyList.size()-1] = 0;

    for(int i =0;i<adjacencyList.size()-1;i++) {
        bool changedInThisIteration = false;
        for(int j =0;j<adjacencyList.size();j++) {
            if(changed[j]) {
                vector<neighbors> outgoingNodes = findOutgoingEdgesNodes(j, adjacencyList);
                for(int k =0; k<outgoingNodes.size();k++) {
                    //For each node, if the memo[incoming node] > memo[j] + weight[incoming node,j],
                    //then set that as the memo value for the incoming node
                    if(memo[j] != INT_MAX && memo[outgoingNodes[k].node] > memo[j] + outgoingNodes[k].weight) {
                        memo[outgoingNodes[k].node] = memo[j] + outgoingNodes[k].weight;
                        successor[outgoingNodes[k].node] = j;
                        changed[outgoingNodes[k].node] = true;
                        changedInThisIteration = true;
                    } else {
                        changed[outgoingNodes[k].node] = false;
                    }
                }
            }
        }
        //If nothing changes in this iteration, we are done
        if(!changedInThisIteration)
            break;
    }

    return successor;

}

int main() {

    vector<vector<neighbors>> adjacencyList = {
        {{1,-4},{5,-3}},
        {{3,-1},{4,-2}},
        {{1,8},{5,3}},
        {{0,6},{5,4}},
        {{2,-3},{5,2}},
        {} 
    };

    vector<int> shortestPathToDest = shortestPath(adjacencyList, 5);
    for(int i =0;i<shortestPathToDest.size();i++)
        cout<<shortestPathToDest[i];
    return 0;
}