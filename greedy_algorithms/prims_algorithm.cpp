#include<vector>
#include<iostream>

using namespace std;

struct neighbors{
    int node, weight;
};

struct graph {
    vector<vector<neighbors>> edges;
    vector<int> degree;
    int nedges,nvertices;    
};

/* Correct the heap from down to up */ 
void heapifyUp(vector<vector<int>> &priorityQueue,int index) {
    if(index > 0) {
        int parent = (index -1)/2;
        if(priorityQueue[parent][1] > priorityQueue[index][1]) {
            swap(priorityQueue[parent], priorityQueue[index]);
            heapifyUp(priorityQueue, parent);
        }
    } 
}

/* Correct the heap from up to down */
void heapifyDown(vector<vector<int>> &priorityQueue, int index) {
    int sizeOfQueue = priorityQueue.size();
    int minChildIndex;
    if((2*index +1) > (sizeOfQueue-1))
        return;
    else if((2*index +1) < (sizeOfQueue-1)) {
        if(priorityQueue[2*index+1][1] < priorityQueue[2*index+2][1]) {
            minChildIndex = 2*index+1;
        } else if(priorityQueue[2*index+1][1] > priorityQueue[2*index+2][1]){
            minChildIndex = 2*index+2;
        }
    }
    else  
        minChildIndex = 2*index + 1;
    if(priorityQueue[index] > priorityQueue[minChildIndex]) {
        swap(priorityQueue[index], priorityQueue[minChildIndex]);
        heapifyDown(priorityQueue, minChildIndex);
    }
}

/* Return the minimum value and then restructure the heap */
vector<int> getMin(vector<vector<int>> &priorityQueue) {
    vector<int> minItem = priorityQueue[0];
    priorityQueue[0] = priorityQueue[priorityQueue.size()-1];
    priorityQueue.resize(priorityQueue.size()-1);
    heapifyDown(priorityQueue,0);
    return minItem;
}

void primsMinimumSpanningTree (graph G, int sourceNode) {
    int numberOfNodes = G.nvertices;
    vector<vector<neighbors>> adjacencyList = G.edges;

    //Create a boolean array of all nodes to check if they are visited
    vector<bool> visited(numberOfNodes, false);

    //Create an array to hold the distance from the explored set. Assign 
    //the max value by default
    vector<int> distanceFromExploredSet(numberOfNodes, 100);

    //Use a a 2d array where the 0 index of every element
    //is the node and the 1 index is the distance from the source
    vector<vector<int>> priorityQueueOfDistanceFromExploredSet;

    //Push the source node into the priority queue
    priorityQueueOfDistanceFromExploredSet.push_back({sourceNode,0});

    distanceFromExploredSet[sourceNode] = 0;

    vector<vector<neighbors>> adjacencyListOfMST(numberOfNodes, vector<neighbors>({}));

    vector<int> backEndOfEdge(numberOfNodes,-1);

    while (!priorityQueueOfDistanceFromExploredSet.empty())
    {
        //Fetch the node with the minimum distance from the source node
        vector<int> minPriorityItem = getMin(priorityQueueOfDistanceFromExploredSet);
        int node = minPriorityItem[0];
        int weight = minPriorityItem[1];

        //If this node has already been visited, skip this iteration
        if(visited[node])
            continue;
        //Otherwise, set the visited of this node to be true
        visited[node] = true;
        distanceFromExploredSet[node] = 0;

        //Loop through all the edges that this node is connected to
        for(int i = 0;i<adjacencyList[node].size();i++) {
            int otherNode = adjacencyList[node][i].node;
            int edgeWeight = adjacencyList[node][i].weight;
            //If the distance of this node from the source + the weight is lesser than the 
            //current distance of the other node from the source, then set the sitance from
            //source of the other node to distance from the source of this node + the edge weight
            //to the other node. Then push it to the priority queue and restructure the heap.
            if(edgeWeight < distanceFromExploredSet[otherNode] && !visited[otherNode]) {
                if(backEndOfEdge[otherNode] != -1) {
                    int otherEdgeCurrentlyInMST = backEndOfEdge[otherNode];
                    for(int j =0;j<adjacencyListOfMST[otherEdgeCurrentlyInMST].size();j++){
                        if(adjacencyListOfMST[otherEdgeCurrentlyInMST][j].node == otherNode) {
                            swap(adjacencyListOfMST[otherEdgeCurrentlyInMST][j], adjacencyListOfMST[otherEdgeCurrentlyInMST][adjacencyListOfMST[otherEdgeCurrentlyInMST].size()-1]);
                            adjacencyListOfMST[otherEdgeCurrentlyInMST].resize(adjacencyListOfMST[otherEdgeCurrentlyInMST].size()-1);
                        }
                    }
                }
                backEndOfEdge[otherNode] = node;
                distanceFromExploredSet[otherNode] =  edgeWeight;
                neighbors treeNode = {otherNode,edgeWeight};
                adjacencyListOfMST[node].push_back(treeNode);
                priorityQueueOfDistanceFromExploredSet.push_back({otherNode, distanceFromExploredSet[otherNode]});
                heapifyUp(priorityQueueOfDistanceFromExploredSet, priorityQueueOfDistanceFromExploredSet.size()-1);
            }
        }
    }

    for(int i =0; i<adjacencyListOfMST.size();i++) {
        cout<<"Node "<<i<<" - ";
        for (int j=0;j<adjacencyListOfMST[i].size();j++)
            cout<<adjacencyListOfMST[i][j].node<<",";
        cout<<endl;
    }
}

int main() {
    vector<vector<neighbors>> adjacencyList = 
    {
        {{1,4}, {7,8}},
        {{2,8}, {7,11}},
        {{3,7}, {8,2},{5,4}},
        {{4,9},{5,14}},
        {{}},
        {{4,10}},
        {{5,2},{8,6}},
        {{6,1},{8,7}},
        {}
    };

    int numOfEdges = 8;
    int numOfVertices = 9;
    vector<int> degree = {2,2,3,1,1,2,2,2,1};
    graph G = {adjacencyList, degree, numOfEdges, numOfVertices};
    primsMinimumSpanningTree(G, 0);
    return 0;
}