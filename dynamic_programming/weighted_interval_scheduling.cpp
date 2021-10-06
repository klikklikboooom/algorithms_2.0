#include<iostream>
#include<vector>

using namespace std;
typedef struct jobs {
    int startTime, finishTime, weight;
} jobs;

/* Find the latest non conflicting job for a particular job j */
int latestNonConflictingJob(vector<jobs> jobList, int j) {
    int low =0, high = j-1;
    while(low <=high) {
        int mid = (low+high)/2;
        if(jobList[mid].finishTime <= jobList[j].startTime) {
            if(jobList[mid+1].finishTime <= jobList[j].startTime)
                low=mid+1;
            else
                return mid;
        }
        else
            high = mid -1;
    }
    return -1;
}

/* Find the partition, i.e, where the pivot element should be in the array when performing quicksort */
int partition(vector<jobs> &arrToSort, int low, int high) {
    int pivot = arrToSort[high].finishTime;
    int i = low -1;
    for(int j = low; j<high;j++) {
        if(arrToSort[j].finishTime<pivot) {
            i++;
            swap(arrToSort[i], arrToSort[j]);
        }
    }
    swap(arrToSort[i+1], arrToSort[high]);
    return i+1;
}

/* Quicksort to sort jobs by finish time */
void sortJobsOnFinishTime(vector<jobs> &jobList, int low, int high) {
    if(low<high) {
        int p = partition(jobList, low, high);
        sortJobsOnFinishTime(jobList, low, p-1);
        sortJobsOnFinishTime(jobList, p+1, high); 
    }   
}

vector<int> scheduleJobs(vector<int> memo, vector<jobs> jobList, int index, vector<int> scheduledJobs) {
    if(index == -1)
        return {};
    else {
        int p = latestNonConflictingJob(jobList, index);
        if(jobList[index].weight + memo[p] >= memo[index-1]) {
            vector<int> recursivelyScheduleJobs =  scheduleJobs(memo, jobList, p, scheduledJobs);
            recursivelyScheduleJobs.push_back(index);
            return recursivelyScheduleJobs;
        } else 
            return scheduleJobs(memo, jobList,index-1, scheduledJobs);
    }
}

pair<vector<int>,int> maxWeightAndSolution(vector<jobs> jobList) {
    sortJobsOnFinishTime(jobList, 0, jobList.size()-1);
    vector<int> memo;
    for(int i = 0; i<jobList.size();i++) {
        if(i==0)
            memo.push_back(jobList[i].weight);
        else 
            memo.push_back(max(jobList[i].weight + memo[latestNonConflictingJob(jobList,i)], memo[i-1]));
    }
        
    vector<int> scheduledJobs = scheduleJobs(memo, jobList, jobList.size()-1, {});
    pair<vector<int>,int> finalSolution = {scheduledJobs, memo[memo.size()-1]};
    return finalSolution;
} 

int main() {
    vector<jobs> jobList = {{3, 10, 20}, {1, 2, 50}, {6, 19, 100}, {2, 100, 200}};
    pair<vector<int>,int> weightAndSchedule = maxWeightAndSolution(jobList);
    cout<<"Optimal Weight "<<weightAndSchedule.second<<endl;
    cout<<"Jobs"<<endl;
    for(int i =0;i<weightAndSchedule.first.size();i++)
        cout<<weightAndSchedule.first[i]<<endl;
    return 0;
}