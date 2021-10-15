#include<iostream>
#include<vector>
#include<map>

using namespace std;

int maxBasePairs(string rnaSequence) {
    int length = rnaSequence.length();
    int memoRows = length-5+1;
    vector<vector<int>> memo(memoRows, vector<int>(length+1));
    map<char,char> basePairMap;

    //Create mapping for base pairs
    basePairMap['A'] = 'U';
    basePairMap['U'] = 'A';
    basePairMap['C'] = 'G';
    basePairMap['G'] = 'C';

    //Set the 0 indices to 0
    for (int i =0;i<=length;i++)
        memo[0][i] = 0;
    
    for(int i =0; i< memo.size();i++)
        memo[i][0] = 0;

    //The first 5 columns for the first 5 rows don't count 
    for(int i =1;i<5;i++) {
        for(int j = 1; j<=5;j++) {
            memo[i][j] = 0;
        }
    }

    //Whenever i-j >= 4, set the table value to 0
    for(int i =1;i<5;i++)
        for(int j = 6; j<=length;j++)
            if((j-i) <= 4)
                memo[i][j] = 0;

    for(int k = 5; k <= length;k++) {
        for(int i =1;i<=(length-k);i++) {
            map<char, int>::iterator it;
            int j = i+k;
            char charAtJ = rnaSequence[j-1];
            auto pairMatchingIterator = basePairMap.find(charAtJ);
            char otherCharOfPair = pairMatchingIterator->second;
            vector<int> pairIndices;
            
            // Loop from j-5 because of no sharp turns condition
            for(int l = j-5; l>=i;l--) {
                //Since indexing of strings start at 0
                if(rnaSequence[l-1] == otherCharOfPair)
                    pairIndices.push_back(l);
            }

            int maxPairMemoCount = 0;
            for(int l =0;l<pairIndices.size();l++) {
                int index = pairIndices[l];
                int thisMemoCount = memo[i][index-1] + 1 + memo[index+1][j-1];
                if(thisMemoCount > maxPairMemoCount) {
                    maxPairMemoCount = thisMemoCount;
                }
            }
            memo[i][j] = max(memo[i][j-1], maxPairMemoCount);
        }
    }
    return memo[1][length];
}

int main() {
    string rnaSequence = "ACCGGUAGU";
    int basePairCount = maxBasePairs(rnaSequence);
    cout<<"Max base pairs : "<<basePairCount<<endl;
    return 0;
}