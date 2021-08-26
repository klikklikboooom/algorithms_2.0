#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<queue>
#define N 4
using namespace std;

void stable_matching(int mPreferences[N][N], int wPreferences[N][N]) {
    
    /* Keep track of all the husbands and the wives */
    int husband[N], wife[N], proposalCount[N], wInversePreferenes[N][N];
    queue<int> freeMen;
    
    /* Initially, all men are free, so freeMenCount is N and all men are in the
       freeMen queue */
    for(int i=0;i<4;i++) 
        freeMen.push(i);

    /* All women and men have no partners at first as no man has made a proposal yet*/
    memset(wife, -1, sizeof(wife));
    memset(husband, -1, sizeof(husband));
    memset(proposalCount, 0, sizeof(proposalCount));

    /* Create an inverse of the womens' preference list for each woman */ 
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            wInversePreferenes[i][wPreferences[i][j]] = j;

    while(!freeMen.empty()) {
        /* Choose free man */
        int m = freeMen.front();
        
        /* 
        For each free man, go through his preferences and propose.
        Break out if woman accepts proposal or if all woman have been
        proposed to.
        */

        while (proposalCount[m] <4 && wife[m] == -1)
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
                freeMen.pop();
                freeMen.push(currentHusband);
            }
            proposalCount[m]++;
        }
    }

    printf("Husband\tWife \n");
    for(int i =0;i<N;i++)
        printf ("%d \t %d'\n", i,wife[i]);    
}

int main() {
    /* Women preferences */
    int wPreferences[N][N] = {{0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3}};

    /* Men preferences */
    int mPreferences[N][N] = { {3, 1, 2, 0},
        {1, 0, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
    };
    stable_matching(mPreferences,wPreferences);
    return 0;
}