#include<iostream>
#include<vector>

using namespace std;

void merge(vector<int> &array, int low, int mid, int high) {
    int leftArraySize = mid-low+1;
    int rightArraySize = high -mid;
    vector<int> rightArray, leftArray;
    for(int i =0;i<rightArraySize;i++)
        rightArray.push_back(array[mid+1+i]);
    
    for(int i=0;i<leftArraySize;i++)
        leftArray.push_back(array[low+i]);

    int leftArrayIterator = 0;
    int rightArrayIterator = 0;
    int indexOfMergedArray = low;
    while(leftArrayIterator<leftArraySize && rightArrayIterator < rightArraySize) {
        if(leftArray[leftArrayIterator] <= rightArray[rightArrayIterator]) {
            array[indexOfMergedArray] = leftArray[leftArrayIterator];
            leftArrayIterator++;
        } else {
            array[indexOfMergedArray] = rightArray[rightArrayIterator];
            rightArrayIterator++;
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
}

void mergeSort(vector<int> &array, int low, int high) {
    if(low>=high)
        return;

    int mid = (low+high)/2;
    mergeSort(array, low,mid);
    mergeSort(array, mid+1, high);
    merge(array,low,mid,high);
}

int main() {

    vector<int> unsortedArray = {12,11,13,5,7,6};
    
    cout<<"Unsorted Array - ";
    for(int i=0;i<unsortedArray.size();i++)
        cout<<unsortedArray[i]<<",";
    mergeSort(unsortedArray,0,unsortedArray.size()-1);
    
    cout<<endl;
    cout<<"Sorted array - ";
    for(int i =0;i<unsortedArray.size();i++) {
        cout<<unsortedArray[i]<<",";
    }
    cout<<endl;

    return 0;
}