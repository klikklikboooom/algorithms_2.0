#include<iostream>
#include<vector>
#include<map>

using namespace std;

/*Calculate what the mismatch penalty should be 
If one character is a consonant and one is a vowel
or vice versa, the penalty is 3. If both are consonants 
or vowels, the penalty is 1. If both are the same character, 
the penalty is 0.
*/
int calculateMatchingPenalty(char a, char b) {
    vector<char> vowels = {'a','e', 'i', 'o', 'u'};
    vector<char> consonants = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',
                               'n', 'p', 'q', 'r', 's', 't', 'v', 'x', 'y','z'};

    if(a == b)
        return 0;
    else if((binary_search(vowels.begin(), vowels.end(), a) && 
    binary_search(vowels.begin(), vowels.end(), b)) ||
    (binary_search(consonants.begin(), consonants.end(), a) && 
    binary_search(consonants.begin(), consonants.end(), b)))
        return 1;
    else
        return 3;
}

int findSmallestPenalty(string a, string b) {
    vector<vector<int>> memo(a.length()+1, vector<int>(b.length()+1));
    int gapPenalty = 2;
    for(int i =0; i <= a.length();i++)
        memo[i][0] = gapPenalty*i;

    for(int i =0; i <= b.length();i++)
        memo[0][i] = gapPenalty*i;

    for(int i =1;i<=a.length();i++) {
        for(int j =1; j<=b.length();j++) {
            int mismatchPenalty = calculateMatchingPenalty(a[i-1], b[j-1]);
            int tempMin = min(mismatchPenalty + memo[i-1][j-1], gapPenalty + memo[i-1][j]);
            memo[i][j] = min(tempMin, gapPenalty + memo[i][j-1]);
            cout<<"nothing";
        }
    } 
    return memo[a.length()][b.length()];    
}

int main() {
    string a = "mean";
    string b = "name";
    int leastPenalty = findSmallestPenalty(a,b);
    cout<<"Smallest Penalty "<<leastPenalty<<endl;
    return 0;
}