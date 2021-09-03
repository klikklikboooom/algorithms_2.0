#include<iostream>
#include<vector>

using namespace std;

/* Find the partition, i.e, where the pivot element should be in the array when performing quicksort */
int partition(vector<int> &arrToSort, vector<int> &otherArr, int low, int high) {
    int pivot = arrToSort[high];
    int i = low-1;
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
void sortDeadlinesAndSwapJobTimes(vector<int> &deadlines, vector<int> &jobTimes,int low, int high) {
    if(low<high) {
        int p = partition(deadlines, jobTimes, low, high);
        sortDeadlinesAndSwapJobTimes(deadlines,jobTimes, low, p-1);
        sortDeadlinesAndSwapJobTimes(deadlines, jobTimes, p+1, high); 
    }   
}

vector<vector<int>> scheduleJobs(vector<int> deadlines, vector<int> jobTimes) {
    int startTime = 0;
    int finishTime = 0;
    vector<vector<int>> scheduledJobs;
    for(int i=0;i<deadlines.size();i++) {
        finishTime+= jobTimes[i];
        scheduledJobs.push_back({startTime, finishTime});
        startTime=finishTime;
    }
    return scheduledJobs; 
}

int calculateLateness(vector<vector<int>> schedule, vector<int> deadlines) {
    int maximumLateness = 0;
    for(int i =0;i<schedule.size();i++)
        maximumLateness += max(0, schedule[i][1] - deadlines[i]);
    
    return maximumLateness;
}

int main() {
    vector<int> jobTimes = {4,3,2,1,2,3};
    vector<int> deadlines = {9,6,15,9,8,14};
    sortDeadlinesAndSwapJobTimes(deadlines, jobTimes, 0, 5);
    vector<vector<int>> scheduledJobs = scheduleJobs(deadlines, jobTimes);
    cout<<"Schedule - ";
    for(int i = 0; i<scheduledJobs.size();i++) 
        cout<<scheduledJobs[i][0]<<"-"<<scheduledJobs[i][1]<<"  ";
    
    cout<<endl;
    
    cout<<calculateLateness(scheduledJobs, deadlines)<<endl;    
    return 0;
}