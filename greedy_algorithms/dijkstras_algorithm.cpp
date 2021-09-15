#include<iostream>
#include<vector>
#define VERTICES 9
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

/* Print the shortest path to each node from the source */
void shortestPathToEachNode(graph G, int sourceNode) {
    int numberOfNodes = G.nvertices;
    vector<vector<neighbors>> adjacencyList = G.edges;

    //Create a boolean array of all nodes to check if they are visited
    vector<bool> visited(numberOfNodes, false);
    
    //Create an array to hold the distance from the source node. Assign the max value by 
    //default
    vector<int> distanceFromSourceNode(numberOfNodes, 100);
    
    //Use a a 2d array where the 0 index of every element
    //is the node and the 1 index is the distance from the source
    vector<vector<int>> priorityQueueOfDistanceFromSource;
    
    //Push the source node into the priority queue
    priorityQueueOfDistanceFromSource.push_back({sourceNode,0});
    
    //Initialize the distance of the source node to the source sourceNode
    //to be 0
    distanceFromSourceNode[sourceNode] = 0;
    
    //Loop as long as the priority queue exists
    while(!priorityQueueOfDistanceFromSource.empty()) {
        //Fetch the node with the minimum distance from the source node
        vector<int> minPriorityItem = getMin(priorityQueueOfDistanceFromSource);
        int node = minPriorityItem[0];
        int weight = minPriorityItem[1];
    
        //If this node has already been visited, skip this iteration
        if(visited[node])
            continue;
        //Otherwise, set the visited of this node to be true
        visited[node] = true;
        
        //Loop through all the edges that this node is connected to
        for(int i = 0;i<adjacencyList[node].size();i++) {
            int otherNode = adjacencyList[node][i].node;
            int edgeWeight = adjacencyList[node][i].weight;
            //If the distance of this node from the source + the weight is lesser than the 
            //current distance of the other node from the source, then set the sitance from
            //source of the other node to distance from the source of this node + the edge weight
            //to the other node. Then push it to the priority queue and restructure the heap.
            if(distanceFromSourceNode[node] + edgeWeight < distanceFromSourceNode[otherNode]) {
                distanceFromSourceNode[otherNode] =  distanceFromSourceNode[node] + edgeWeight;
                priorityQueueOfDistanceFromSource.push_back({otherNode, distanceFromSourceNode[otherNode]});
                heapifyUp(priorityQueueOfDistanceFromSource, priorityQueueOfDistanceFromSource.size()-1);
            }
        }
    }
    for(int i =0; i<distanceFromSourceNode.size();i++) {
        cout<<"Node "<<i<<" Distance from source - "<<distanceFromSourceNode[i]<<endl;
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

    int numOfEdges = 14;
    int numOfVertices = 9;
    vector<int> degree = {2,2,3,1,1,2,2,2,1};
    graph G = {adjacencyList, degree, numOfEdges, numOfVertices};
    shortestPathToEachNode(G,0);
    return 0;
}