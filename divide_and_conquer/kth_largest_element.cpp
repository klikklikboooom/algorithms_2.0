#include<iostream>
#include<vector>

using namespace std;
vector<vector<int>> partitionList(vector<int> list, int numPartitions) {
    int sizeOfList = list.size();
    int partitionSize = sizeOfList/numPartitions;
    vector<vector<int>> partitionedLists(numPartitions);
    for(int i =0; i<numPartitions;i++) {
        int startingPosition = partitionSize*i;
        if(i < numPartitions-1) 
            for(int j =startingPosition;j<startingPosition+partitionSize;j++)
                partitionedLists[i].push_back(list[j]);
        else {
            for(int j =startingPosition;j< sizeOfList;j++)
                partitionedLists[i].push_back(list[j]);
        }
    }
    return partitionedLists;
}

/* Find the partition, i.e, where the pivot element should be in the array when performing quicksort */
int partition(vector<int> &arrToSort,int low, int high) {
    int pivot = arrToSort[high];
    int i = low-1;
    for(int j = low; j<high;j++) {
        if(arrToSort[j]<pivot) {
            i++;
            swap(arrToSort[i], arrToSort[j]);
        }
    }
    swap(arrToSort[i+1], arrToSort[high]);
    return i+1;
}

/* Quicksort to sort finish times and move associated start times */
void sortList(vector<int> &list, int low, int high) {
    if(low<high) {
        int p = partition(list, low, high);
        sortList(list, low, p-1);
        sortList(list,  p+1, high); 
    }   
}

int kthLargestElement(vector<int> list, int k) {
    if(list.size() <= 5) {
        vector<int> copyOfList = list;
        sortList(copyOfList, 0, copyOfList.size()-1);
        return copyOfList[copyOfList.size() - k];
    }
    
    int numPartitions = 5;
    vector<vector<int>> partitionedLists = partitionList(list, numPartitions);
    vector<int> medians;
    int sizeOfPartition = list.size()/numPartitions;
    
    for(int i =0;i<partitionedLists.size();i++) {
        //Since we are recursively calling this function to find medians, find the k value 
        //for the median element
        int medianKValue;
        if(i < partitionedLists.size() - 1)             
            medianKValue = sizeOfPartition/2 + 1;
        else
            medianKValue = partitionedLists[partitionedLists.size()-1].size()/2 + 1; 
        int thisMedian = kthLargestElement(partitionedLists[i], medianKValue);
        medians.push_back(thisMedian);
    }

    int kForMedianOfMedians = medians.size()/2 + 1;
    int medianOfMedians = kthLargestElement(medians, kForMedianOfMedians);
    vector<int> leftSide, rightSide;
    for(int i =0;i<list.size();i++) {
        if(list[i] < medianOfMedians)
            leftSide.push_back(list[i]);
        else
            rightSide.push_back(list[i]);
    }

    if(k <= rightSide.size())
        return kthLargestElement(rightSide, k);
    else if(k>=rightSide.size() + 2)
        return kthLargestElement(leftSide, k - 1 - rightSide.size());
    else 
        return medianOfMedians;
}

int main() {
    vector<int> list = {5, 2, 17, 13, 14, 19, 14, 18, 4, 8, 0, 6, 12, 10};
    int k = 3;
    int element = kthLargestElement(list, k);
    cout<<k<<"rd largest element = "<<element<<endl;
    return 0;
}