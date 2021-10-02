#include<iostream>
#include<vector>

using namespace std;

int mergeAndCount(vector<int> &array, int low, int mid, int high) {
    int leftArraySize = mid-low+1;
    int rightArraySize = high -mid;
    vector<int> leftArray, rightArray;
    
    for(int i =0;i<leftArraySize;i++)
        leftArray.push_back(array[low+i]);
    
    for(int i=0;i<rightArraySize;i++)
        rightArray.push_back(array[mid+i+1]); 
    
    int leftArrayIterator = 0;
    int rightArrayIterator = 0;
    int indexOfMergedArray = low;
    int inversionCount = 0;

    while (leftArrayIterator < leftArraySize && rightArrayIterator < rightArraySize)
    {
        if(leftArray[leftArrayIterator] <= rightArray[rightArrayIterator]) {
            array[indexOfMergedArray] = leftArray[leftArrayIterator];
            leftArrayIterator++;
        } else {
            array[indexOfMergedArray] = rightArray[rightArrayIterator];
            rightArrayIterator++;
            //Add the all the remaining elements on the left side if the right side 
            //element is greater
            inversionCount += leftArraySize - leftArrayIterator;
        }
        indexOfMergedArray++;
    }

    while(leftArrayIterator<leftArraySize) {
        array[indexOfMergedArray] = leftArray[leftArrayIterator];
        leftArrayIterator++;
        indexOfMergedArray++;
    }

    while(rightArrayIterator<rightArraySize) {
        array[indexOfMergedArray] = rightArray[rightArrayIterator];
        rightArrayIterator++;
        indexOfMergedArray++;
    }
    return inversionCount;
}

int sortAndCount(vector<int> &array, int low, int high){
    if(low>=high)
        return 0;
    int mid = (high+low)/2;
    int leftSideInversions = sortAndCount(array,low, mid);
    int rightSideInversions = sortAndCount(array,mid+1,high);
    int inversionCount = mergeAndCount(array,low,mid,high);
    return (rightSideInversions + leftSideInversions + inversionCount);
}

int main() {
    
    vector<int> unsortedArray = {12,11,13,5,7,6};
    cout<<"Unsorted Array - ";
    for(int i=0;i<unsortedArray.size();i++)
        cout<<unsortedArray[i]<<",";
    int inversionCount = sortAndCount(unsortedArray,0,unsortedArray.size()-1);
    
    cout<<endl;
    cout<<"Sorted array - ";
    for(int i =0;i<unsortedArray.size();i++) {
        cout<<unsortedArray[i]<<",";
    }
    cout<<endl;
    cout<<"Number of inversions - "<<inversionCount<<endl;
    return 0;
}