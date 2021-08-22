#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define N 4

bool changePartner(int preferences[2*N][N], int woman, int man1, int man2) {
    printf("%d\n", man1);
    printf("%d\n", man2); 
    for(int i=0;i<N;i++) {

        /* 
        man1 is the current partner of the woman. If the current partner 
        is higher in the preference list, then return true which will do 
        nothing
        */
        if(preferences[woman][i] == man1)
            return false;

        /*
        man2 is the new man proposing. If this man is higher in the preference
        list, return false which will change the partner
        */
        if(preferences[woman][i] == man2)
            return true;
 
    }
}

void stable_matching(int preferences[2*N][N]) {
    int freeMenCount = N;
    int womenPartners[N];
    bool freeMen[N];
    /* All women have no partners and all men are free at first */
    memset(womenPartners, -1, sizeof(womenPartners));
    memset(freeMen, true, sizeof(freeMen));

    while(freeMenCount > 0) {
        int m;
        /* Search for a free man */
        for(m=0;m<N;m++){
            if(freeMen[m])
                break;            
        }
        
        /* 
        For each free man, go through his preferences and propose.
        Break out if woman accepts proposal 
        */
        for(int i =0;i<N && freeMen[m] == true; i++) {
            int highestWomanPreference = preferences[m][i];

            /* 
            m1 is the current partner, if any and m is the new
            proposing to the woman
            */

            int m1 = womenPartners[highestWomanPreference-N];
            
            /* If free, propose and get engaged */
            if(m1 == -1) {
                womenPartners[highestWomanPreference-N] = m;
                freeMenCount--;
                freeMen[m] = false;
            } else {
                /* 
                Check to see if the partner needs to be changed based on preferences.
                If so, change the partner to m and m1 becomes free
                */
                if(changePartner(preferences, highestWomanPreference, m1, m)){
                    womenPartners[highestWomanPreference-N] = m;
                    freeMen[m] = false;
                    freeMen[m1] = true;
                }
            }
        }   
    }

    for(int i =0;i<N;i++)
        printf ("%d \t %d\n", i+N,womenPartners[i]);    
}

int main() {
    /* First 4 preferences are mens, last 4 are womens */
    int preferences[2*N][N] = { {7, 5, 6, 4},
        {5, 4, 6, 7},
        {4, 5, 6, 7},
        {4, 5, 6, 7},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
        {0, 1, 2, 3},
    };
    stable_matching(preferences);
    return 0;
}