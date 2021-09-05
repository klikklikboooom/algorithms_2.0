#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<queue>
#include<vector>
#define N 3
using namespace std;

void stable_matching(vector<vector<int>> mPreferences, vector<vector<int>> wPreferences) {
    
    /* 
    Keep track of the husbands and the wives. Set to -1 as no one has a  
    partner at first. Also, set the proposal count to 0 for all men.
    */
    vector<int> husband(N,-1), wife(N,-1), proposalCount(N,0);
    vector<vector<int>> wInversePreferenes(N,vector<int>(N));
    queue<int> freeMen;
    
    /* Initially, all men are free, so freeMenCount is N and all men are in the
       freeMen queue */
    for(int i=0;i<N;i++) 
        freeMen.push(i);

    /* Create an inverse of the womens' preference list for each woman */ 
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            wInversePreferenes[i][wPreferences[i][j]] = j;
    
    int totalProposalCount =0;

    while(!freeMen.empty()) {
        /* Choose free man */
        int m = freeMen.front();
        
        /* 
        For each free man, go through his preferences and propose.
        Break out if woman accepts proposal or if all woman have been
        proposed to.
        */

        while (proposalCount[m] <N && wife[m] == -1)
        {   
            /* 
            Get the free mans' highest preference that he hasn't proposed to yet
            and her current husband (will be 0 if she doesn't have one yet) 
            */
            int woman = mPreferences[m][proposalCount[m]];
            int currentHusband = husband[woman];
            
            /*
            If there is no current husband, then the two get engaged and the man is 
            no longer free 
            */
            if(currentHusband == -1) {
                husband[woman] = m;
                wife[m] = woman;
                freeMen.pop();
            } 
            else if(wInversePreferenes[woman][m] < wInversePreferenes[woman][currentHusband]) {
            /* 
            Otherwise, the the woman checks the inverse list. In the inverse list, if the value
            stored for the free man is lesser than the current husbands value, the woman elopes
            with the free man and the current husband is free.
            */
                husband[woman] = m;
                wife[m] = woman;
                wife[currentHusband] = -1;
                freeMen.pop();
                freeMen.push(currentHusband);
            }
            proposalCount[m]++;
            totalProposalCount++;
        }
    }
    printf("Husband\tWife \n");
    for(int i =0;i<N;i++)
        printf ("%d \t %d'\n", i,wife[i]);    
}

int main() {
    /* Women preferences */
    vector<vector<int>> wPreferences = {{1,2,0},
        {0,1,2},
        {2,0,1}};

    /* Men preferences */
    vector<vector<int>> mPreferences = { {0,1,2},
        {1,0,2},
        {1,0,2}};
    stable_matching(mPreferences,wPreferences);
    return 0;
}