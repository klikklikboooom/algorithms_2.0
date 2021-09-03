#include<iostream>
#include<vector>

using namespace std;

struct classRoomLastFinishTime {
    int classRoomNumber, lastFinishTime;
};

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
void sortStartTimesandSwapFinishTimes(vector<int> &startTimes, vector<int> &finishTimes,int low, int high) {
    if(low<high) {
        int p = partition(startTimes, finishTimes, low, high);
        sortStartTimesandSwapFinishTimes(startTimes,finishTimes, low, p-1);
        sortStartTimesandSwapFinishTimes(startTimes, finishTimes, p+1, high); 
    }   
}

/* Correct the heap from down to up */ 
void heapifyUp(vector<classRoomLastFinishTime> &priorityQueue,int index) {
    if(index > 0) {
        int parent = (index -1)/2;
        if(priorityQueue[parent].lastFinishTime > priorityQueue[index].lastFinishTime) {
            swap(priorityQueue[parent], priorityQueue[index]);
            heapifyUp(priorityQueue, parent);
        }
    } 
}

/* Correct the heap from up to down */
void heapifyDown(vector<classRoomLastFinishTime> &priorityQueue, int index) {
    int sizeOfQueue = priorityQueue.size();
    int minChildIndex;
    if((2*index +1) > (sizeOfQueue-1))
        return;
    else if((2*index +1) < (sizeOfQueue-1)) {
        if(priorityQueue[2*index+1].lastFinishTime < priorityQueue[2*index+2].lastFinishTime) {
            minChildIndex = 2*index+1;
        } else if(priorityQueue[2*index+1].lastFinishTime > priorityQueue[2*index+2].lastFinishTime){
            minChildIndex = 2*index+2;
        }
    }
    else  
        minChildIndex = 2*index + 1;
    if(priorityQueue[index].lastFinishTime > priorityQueue[minChildIndex].lastFinishTime) {
        swap(priorityQueue[index], priorityQueue[minChildIndex]);
        heapifyDown(priorityQueue, minChildIndex);
    }
}

/* Generate a 3D array consisting of each classrooms' lectures' start and finish time */
vector<vector<vector <int> > > assignLecturesToClassrooms(vector<int> startTimes,
                                                            vector<int> finishTimes) {
    int depth = 0;
    vector<vector<vector<int> > > classRoomsWithLectures;
    vector<classRoomLastFinishTime> classroomFinishTimesPriorityQueue;

    for(int i=0;i<startTimes.size();i++) {
        /*
        If the current start time is greater than the minimum finish time in the heap,
        then add this lecture to the class with the minumum finish time, change the min 
        heap value to the new finish time and heapify down. 
        Otherwise, create a new classroom, add this lecture, add the finish time to the
        end of the heap and heapify up.
        */
        if(classRoomsWithLectures.size()>0 &&
        startTimes[i] >=classroomFinishTimesPriorityQueue[0].lastFinishTime) {
            classRoomsWithLectures[classroomFinishTimesPriorityQueue[0].classRoomNumber]
            .push_back({startTimes[i], finishTimes[i]});
            classroomFinishTimesPriorityQueue[0].lastFinishTime = finishTimes[i];
            heapifyDown(classroomFinishTimesPriorityQueue,0);
        } else {
            classRoomsWithLectures.push_back({});
            classRoomsWithLectures[depth].push_back({startTimes[i], finishTimes[i]});
            classRoomLastFinishTime newClassRoom = {
                .classRoomNumber = depth, 
                .lastFinishTime = finishTimes[i] 
            };
            depth++;
            classroomFinishTimesPriorityQueue.push_back(newClassRoom);
            heapifyUp(classroomFinishTimesPriorityQueue, classroomFinishTimesPriorityQueue.size()-1);
        }
    }
    return classRoomsWithLectures;
}

int main() {
    
    vector<int> classRoomPriorityQueue;
    vector<int> startTimes = {0,4,5,2,0,6,2,6,4,0};
    vector<int> finishTimes = {2,6,8,4,4,8,5,8,6,2};
    sortStartTimesandSwapFinishTimes(startTimes, finishTimes, 0, 9);
    vector<vector<vector<int> > > classRoomWithLectures = assignLecturesToClassrooms(startTimes, finishTimes);
    for(int i=0;i<classRoomWithLectures.size();i++) {
        cout<<"Classroom "<<i<<" - ";
        for(int j=0; j<classRoomWithLectures[i].size();j++) {
            cout<<classRoomWithLectures[i][j][0]<<"-"<<classRoomWithLectures[i][j][1]<< " ";
        }
        cout<<endl;
    }
    return 0;
}