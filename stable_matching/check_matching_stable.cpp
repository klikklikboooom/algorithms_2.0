#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<array>
#include<vector>
#include<string>
#define N 4

using namespace std;

/* Fetch rank of woman based on preferences */
int wRank(vector<int> preferences, int woman) {
    for(int i=0;i<N;i++)
        if(preferences[i] == woman)
            return i;
    return -1;
}

/* Fetch partition value for quicksort */
int partition(vector<vector<int>> &arr, int low, int high) {
    int pivot = arr[high][1];
    int i = low-1;
    for(int j = low; j<=high-1;j++) {
        if(arr[j][1] < pivot) {
            i++;
            arr[i].swap(arr[j]);
        }
    }
    arr[i+1].swap(arr[high]);
    return i+1;
}

/* Sort 2D array based on every rows 1 index using quicksort*/ 
void sortMatchingBasedOnWomen(vector<vector<int> > &arr, int low, int high) {
    if(low<high) {
        int p = partition(arr, low, high);
        sortMatchingBasedOnWomen(arr, low, p-1);
        sortMatchingBasedOnWomen(arr, p+1, high);
    }
}
/* Returns true or false based on whether matching is stable or not */
bool matching_stable(vector<vector<int> > menMatching, vector<vector<int> > mPreferences,vector<vector<int> > wPreferences) {

    /* Generate an inverse of the womans preference list */
    vector<vector<int> > wInversePreferenes(N, vector<int>(N));
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            wInversePreferenes[i][wPreferences[i][j]] = j;

    /*  
    Create a copy of the matching based on mans sorted order.
    Then, sort based on woman 
    */ 
    vector<vector<int> > womenMatching(N, vector<int>(0,0));
    for(int i=0;i<N;i++)
        for(int j=0;j<2;j++)
            womenMatching[i].push_back(menMatching[i][j]);

    sortMatchingBasedOnWomen(womenMatching,0,3);

    /* 
    For each man, get the rank of his wife.
    Then check with each of the higher ranked women if they can elope.
    If so, return false. If you break out of the loop, return true.
    */
    for(int i=0;i<N;i++) {
        int man = menMatching[i][0];
        int woman = menMatching[i][1];
        int rankOfWife = wRank(mPreferences[i], woman);
        for(int j=0;j<rankOfWife;j++) {
            int higherPrefWoman = mPreferences[i][j];
            int currentHusband = womenMatching[higherPrefWoman][0];
            if(wInversePreferenes[woman][man] < wInversePreferenes[woman][currentHusband])
                return false;
        }
    }

    return true;
}

int main(){
    /* Matching based on order of man */
    vector<vector<int> > matching = {{0,3}, {1,1}, {2,0}, {3, 2}}; 

    /* Women preferences */
    vector<vector<int> > wPreferences = {{0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3}};

    /* Men preferences */
    vector<vector<int> > mPreferences = { {3, 1, 2, 0},
        {1, 0, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
    };

    bool isMatchingStable = matching_stable(matching, mPreferences, wPreferences);

    printf(isMatchingStable? "Matching is stable\n" : "Matching is unstable\n");

    return 0;
}