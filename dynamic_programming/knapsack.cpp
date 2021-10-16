#include<iostream>
#include<vector>

using namespace std;

typedef struct item {
    int value, weight;
} item;

pair<int, vector<int>> findoptimalSubset(vector<item> items, int knapsackWeight) {
    vector<vector<int>> weightItemValueMemo(items.size()+1, vector<int>(knapsackWeight+1));
    
    //Set the 0 index values to 0
    for(int j=0;j<=knapsackWeight;j++)
        weightItemValueMemo[0][j] = 0;


    for(int i =1;i<=items.size();i++) {
        for(int j=1;j<=knapsackWeight;j++) {
            if(items[i].weight > j)
                weightItemValueMemo[i][j] = weightItemValueMemo[i-1][j];
            else 
                weightItemValueMemo[i][j] = max(weightItemValueMemo[i-1][j], items[i].value + weightItemValueMemo[i-1][j - items[i].weight]);
        }
    }

    int i= items.size()-1;
    int weight = knapsackWeight;
    int result = weightItemValueMemo[items.size()][knapsackWeight];
    vector<int> finalItems;
    while(i>=0 && result>=0) {
        if(i > 0) {
            if(weightItemValueMemo[i-1][weight] == result) {
                i--;
                continue;
            } else {
                finalItems.push_back(items[i].value);
                weight -= items[i].weight;
                result -= items[i].value;
            }
        }
        i--;
    }
    pair<int, vector<int>> finalSolution = {weightItemValueMemo[items.size()][knapsackWeight], finalItems};

    return finalSolution;
}


int main() {
    vector<item> allItems = {{0,0},{1,1},{6,2}, {18,5}, {22,6}, {28,7}};
    int knapsackWeight = 11;

    pair<int, vector<int>> finalSolution = findoptimalSubset(allItems, knapsackWeight);
    cout<<"Optimal weight : " << finalSolution.first << endl;
    cout<<"Values "<<endl;
    for (int i = 0;i < finalSolution.second.size();i++) {
        cout<< finalSolution.second[i] << " ";
    cout<<endl;
    }
    return 0;
}