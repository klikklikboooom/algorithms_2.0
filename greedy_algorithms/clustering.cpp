#include<vector>
#include<iostream>

using namespace std;

struct neighbors{
    int node, weight;
};

struct edgesAndWeights {
    int source, dest, weight;
};

struct graph {
    vector<vector<neighbors>> edges;
    vector<int> degree;
    int nedges,nvertices;    
};

struct linkedListNode {
    int data;
    linkedListNode *next;
};

/* Fetch partition value for quicksort */
int partition(vector<edgesAndWeights> &arr, int low, int high) {
    int pivot = arr[high].weight;
    int i = low-1;
    for(int j = low; j<=high-1;j++) {
        if(arr[j].weight < pivot) {
            i++;
            swap(arr[i],arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1;
}

/* Sort arrays of struct containing the edge weights index using quicksort*/ 
void sortEdgeBasedOnWeight(vector<edgesAndWeights> &arr, int low, int high) {
    if(low<high) {
        int p = partition(arr, low, high);
        sortEdgeBasedOnWeight(arr, low, p-1);
        sortEdgeBasedOnWeight(arr, p+1, high);
    }
}

/* For each node create a pointer to itself */
vector<linkedListNode*> createTrees(graph G) {
    int numNodes = G.nvertices;
    vector<linkedListNode*> nodes;
    for(int i=0;i<numNodes;i++) {
        struct linkedListNode* newNode = new linkedListNode;
        newNode->data = i;
        newNode->next = newNode;
        nodes.push_back(newNode);
    }
    return nodes;
}

/* Find the parent of a node */
int find(linkedListNode* node) {
    linkedListNode* ptr = node;
    while(ptr->next!=ptr) {
        ptr = ptr->next;
    }
    linkedListNode* otherPtr = node
    ;
    //After the parent has been found, change the parent of all other nodes
    //on the path of this node to the parent of this node (After all, they have the
    //same parent) 
    while(otherPtr->next!=otherPtr) {
        linkedListNode* nextOtherPtr = otherPtr->next;
        otherPtr->next = ptr;
        otherPtr=nextOtherPtr;
    }
    return ptr->data;
}

/* Perform union operation */
vector<vector<int>> findUnion(graph G, int numClusters) {
    vector<linkedListNode*> trees = createTrees(G);
    //Maintain the rank (count of children) for each node
    vector<int> rank(trees.size(),1);
    vector<vector<neighbors>> adjacencyList = G.edges;
    vector<vector<neighbors>> adjacencyListOfMST(G.nvertices,{});
    vector<edgesAndWeights> edges;

    // Put all the edges into a single list and sort it based on their weight
    for(int i =0;i<adjacencyList.size();i++) {
        for(int j=0; j<adjacencyList[i].size();j++) {
            edges.push_back({i,adjacencyList[i][j].node,adjacencyList[i][j].weight});
        }
    }
    sortEdgeBasedOnWeight(edges, 0, edges.size()-1);

    int numClustersRightNow = G.nvertices;
    int i =0;
    while(numClustersRightNow > numClusters) {
        // For each edge, find the source's and destination's parent 
        int source = edges[i].source;
        int dest = edges[i].dest;
        int sourceParent = find(trees[source]);
        int destParent = find(trees[dest]);
        int finalParent;
        // If the source and destination parent are different, then the 
        // two nodes are in different components and a union needs to be 
        // performed. If the destination has a higher rank (more children),
        // change the pointer of the source parent to point to the destination's
        // parent. If not, do the opposite. Then add the edge to the MST.
        if(sourceParent != destParent) {
            if(rank[destParent] > rank[sourceParent]) {
                trees[sourceParent]->next = trees[destParent];
                rank[destParent]++;
                finalParent = destParent;
            } else {
                trees[destParent]->next = trees[sourceParent];
                rank[sourceParent]++;
                finalParent = sourceParent;
            }
            numClustersRightNow--;
        }
        i++;
    }


    //TODO : Find a way to display the clusters
    // vector<int> clusterParents(numClusters);

    // int oldMax=0;
    // for(int k=0;k<numClusters;k++) {
    //     int maxChildren =0;
    //     int maxIndex = -1;
    //     for(int j=0; j<rank.size();j++) {
    //         if(oldMax == 0) {
    //             if(rank[j]>=maxChildren) {
    //                 maxIndex = j;
    //                 maxChildren = rank[j];
    //             } 
    //         } else if(rank[j] >= maxChildren && rank[j] < oldMax){
    //             maxIndex = j;
    //             maxChildren = rank[j];
    //         }
    //     }
    //     oldMax = maxChildren;
    //     clusterParents[k]= maxIndex;
    // }
    
    vector<vector<int>> clusters(numClusters,{});
    // for(int k=0; k<numClusters;k++) {
    //     int parent = clusterParents[k];
    //     for(int j=0;j<G.nvertices;j++){
    //         int thisNodesParent = find(trees[j]);
    //         if(thisNodesParent == parent)
    //             clusters[k].push_back(j);
    //     }
    // }
        
    return clusters;
}

int main() {

    vector<vector<neighbors>> adjacencyList = 
    {
        {{1,4}, {7,8}},
        {{7,11},{2,8}},
        {{3,7}, {8,2},{5,4}},
        {{4,9},{5,14}},
        {},
        {{4,10}},
        {{5,2},{8,6}},
        {{6,1},{8,7}},
        {}
    };

    int numOfEdges = 14;
    int numOfVertices = 9;
    vector<int> degree = {2,2,3,2,0,1,2,2,0};
    graph G = {adjacencyList, degree, numOfEdges, numOfVertices};
    int numberOfClusters = 3;
    vector<vector<int>> clusters = findUnion(G, numberOfClusters);
    int totalCost = 0;
    for(int i=0;i<clusters.size();i++) {
        cout<<"Cluster "<<i+1<<" - ";
        for(int j=0;j<clusters[i].size();j++) {
            cout<<clusters[i][j]<<",";
        }
        cout<<endl;
    }
    return 0;
}