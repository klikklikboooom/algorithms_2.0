#include<iostream>
#include<vector>
#include<map>
using namespace std;

struct cacheFirstItemIndexInRequestQueue {
    int index, item;
};

/* Correct the heap from down to up */ 
void heapifyUp(vector<cacheFirstItemIndexInRequestQueue> &priorityQueue,int index) {
    if(index > 0) {
        int parent = (index -1)/2;
        if(priorityQueue[parent].index < priorityQueue[index].index) {
            swap(priorityQueue[parent], priorityQueue[index]);
            heapifyUp(priorityQueue, parent);
        }
    } 
}

/* Correct the heap from up to down */
void heapifyDown(vector<cacheFirstItemIndexInRequestQueue> &priorityQueue, int index) {
    int sizeOfQueue = priorityQueue.size();
    int minChildIndex;
    if((2*index +1) > (sizeOfQueue-1))
        return;
    else if((2*index +1) < (sizeOfQueue-1)) {
        if(priorityQueue[2*index+1].index < priorityQueue[2*index+2].index) {
            minChildIndex = 2*index+1;
        } else if(priorityQueue[2*index+1].index > priorityQueue[2*index+2].index){
            minChildIndex = 2*index+2;
        }
    }
    else  
        minChildIndex = 2*index + 1;
    if(priorityQueue[index].index < priorityQueue[minChildIndex].index) {
        swap(priorityQueue[index], priorityQueue[minChildIndex]);
        heapifyDown(priorityQueue, minChildIndex);
    }
}

/* Correct the heap from down to up */ 
void heapifyUp(vector<int> &priorityQueue,int index) {
    if(index > 0) {
        int parent = (index -1)/2;
        if(priorityQueue[parent] < priorityQueue[index]) {
            swap(priorityQueue[parent], priorityQueue[index]);
            heapifyUp(priorityQueue, parent);
        }
    } 
}

/* Correct the heap from up to down */
void heapifyDown(vector<int> &priorityQueue, int index) {
    int sizeOfQueue = priorityQueue.size();
    int minChildIndex;
    if((2*index +1) > (sizeOfQueue-1))
        return;
    else if((2*index +1) < (sizeOfQueue-1)) {
        if(priorityQueue[2*index+1] < priorityQueue[2*index+2]) {
            minChildIndex = 2*index+1;
        } else if(priorityQueue[2*index+1] > priorityQueue[2*index+2]){
            minChildIndex = 2*index+2;
        }
    }
    else  
        minChildIndex = 2*index + 1;
    if(priorityQueue[index] < priorityQueue[minChildIndex]) {
        swap(priorityQueue[index], priorityQueue[minChildIndex]);
        heapifyDown(priorityQueue, minChildIndex);
    }
}

int isItemInCache(vector<int> cache, int item) {
    for(int i=0;i<cache.size();i++)
        if(item == cache[i])
            return true;
    return false;
}

vector<int> children(vector<int> priorityQueue, int parentIndex) {
    if((2*parentIndex+1) > (priorityQueue.size()-1))
        return {-1,-1};
    else if((2*parentIndex+2) > (priorityQueue.size()-1))
        return {priorityQueue[2*parentIndex+1], -1};
    else
        return {priorityQueue[2*parentIndex+1], priorityQueue[2*parentIndex+2]};
}

vector<int> heapsort(vector<int> heap) {
    vector<int> sorted;
    for(int i =0;i<heap.size();i++)
        sorted[i] = getMax(heap);

    return sorted;       
}

int getMax(vector<int> heap) {
    int max = heap[0];
    heap[0] = heap.size()-1;
    heapifyDown(heap, 0);
    return max;
}

int binarySearch(vector<int> array, int left, int right, int number ) {
    if(right >= left) {
        int mid = (left + right)/2;
        if(array[mid] == number)
            return mid;
        
        if(array[mid] > number)
            return binarySearch(array, left, mid-1, number);

        return binarySearch(array, mid+1, right, number); 
    }
    return -1;
}

vector<int> evictions(vector<int> requests, int cacheSize) {
    vector<int> evictions;
    //Maintain the max heap of all items in the cache with respect to 
    //the index of their request.
    vector<cacheFirstItemIndexInRequestQueue> cacheItemRequestPriorityQueue;
    vector<int> maxHeapCacheElements;
    map<int, vector<int>>;
    int  i = 0;
    int requestSize = requests.size();
    while(i<cacheSize) {
        int j = i+1;
        int index = requestSize+1;
        while(j<requestSize) {
            if(requests[i]==requests[j]) {
                index = j;
                break;
            }
        }
        cacheItemRequestPriorityQueue.push_back({index, requests[i]});
        maxHeapCacheElements.push_back(requests[i]);
        heapifyUp(cacheItemRequestPriorityQueue, cacheItemRequestPriorityQueue.size()-1);
        heapifyUp(maxHeapCacheElements,maxHeapCacheElements.size()-1);
    }

    vector<int> sortedCacheItems = heapsort(maxHeapCacheElements);

    while(i<requestSize) {
        int indexOfRequestInCache = binarySearch(sortedCacheItems, 0, sortedCacheItems.size()-1, requests[i]);
        if(indexOfRequestInCache == -1) {
            int j = i+1;
            int index = requestSize+1;
            while(j<requestSize) {
                if(requests[i]==requests[j]) {
                    index = j;
                    break;
                }
            }
            cacheFirstItemIndexInRequestQueue itemToBeEvicted = cacheItemRequestPriorityQueue[0];
            int cacheItemToBeEvicted = binarySearch(sortedCacheItems,0, sortedCacheItems.size()-1, itemToBeEvicted.index);
            evictions.push_back(sortedCacheItems[cacheItemToBeEvicted]);
            sortedCacheItems[cacheItemToBeEvicted] = requests[i];  
            cacheItemRequestPriorityQueue.push_back({index, requests[i]});
            maxHeapCacheElements.push_back(requests[i]);
            heapifyUp(sortedCacheItems, cacheItemToBeEvicted);
            heapifyDown(sortedCacheItems, cacheItemToBeEvicted);
            sortedCacheItems = heapsort(sortedCacheItems);
        }
    }
    return evictions;
}

int main() {
    vector<int> requests = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };
    int cacheSize = 4;
    vector<int> evictionList = evictions(requests, cacheSize);
    cout<<evictionList.size()<<endl;
    cout<<"Items evicted - ";
    for(int i = 0; i<evictionList.size();i++)
        cout<<evictionList[i]<<",";

    cout<<endl;
    return 0;
    
}