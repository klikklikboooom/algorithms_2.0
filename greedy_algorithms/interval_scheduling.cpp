#include<iostream>
#include<vector>

using namespace std;

/* Find the partition, i.e, where the pivot element should be in the array when performing quicksort */
int partition(vector<int> &arrToSort, vector<int> &otherArr, int low, int high) {
    int pivot = arrToSort[high];
    int i = low -1;
    for(int j = low; j<high;j++) {
        if(arrToSort[j]<pivot) {
            i++;
            swap(arrToSort[i], arrToSort[j]);
            swap(otherArr[i], otherArr[j]);
        }
    }
    swap(arrToSort[i+1], arrToSort[high]);
    swap(otherArr[i+1],  otherArr[high]);
    return i+1;
}

/* Quicksort to sort finish times and move associated start times */
void sortFinishTimesandSwapStartTimes(vector<int> &finishTimes, vector<int> &startTimes, int low, int high) {
    if(low<high) {
        int p = partition(finishTimes, startTimes, low, high);
        sortFinishTimesandSwapStartTimes(finishTimes,startTimes, low, p-1);
        sortFinishTimesandSwapStartTimes(finishTimes, startTimes, p+1, high); 
    }   
}

/*Pick the jobs that are compatible */
vector<int> pickJobs(vector<int> sortedFinishTimes, vector<int> mappedStartTimes) {
    vector<int> jobs = {0};
    int index = 0;
    for(int i=1;i<sortedFinishTimes.size(); i++) {
        if(mappedStartTimes[i] > sortedFinishTimes[index]) {
            index=i;
            jobs.push_back(i);
        }
    }
    return jobs;
}

int main() {
    vector<int> s =  {3, 1, 5, 0, 8, 5};
    vector<int> f=  {4, 2, 7, 6, 9, 9};
    sortFinishTimesandSwapStartTimes(f,s,0,5);
    vector<int> jobs = pickJobs(f,s);
    cout<<"Start Time\tFinish Time"<<endl;
    for(int i = 0;i< jobs.size();i++)
        cout<<s[jobs[i]]<<"\t\t"<<f[jobs[i]]<<endl;
    return 0;
}

//s - [1,3,0,5,8,5]
//f - [2,4,6,7,9,9]
//A - [0,1,3,5]