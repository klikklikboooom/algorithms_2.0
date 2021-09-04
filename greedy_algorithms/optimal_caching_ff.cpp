#include<iostream>
#include<vector>

using namespace std;

bool isItemInCache(vector<int> cache, int item) {
    for(int i=0;i<cache.size();i++)
        if(item == cache[i])
            return true;
    return false;
}

vector<int> evictions(vector<int> requests, int cacheSize) {
    vector<int> evictions, cache;
    int  i = 0;
    while(i<cacheSize) {
        cache.push_back(requests[i]);
        i++;
    }

    while(i<requests.size()) {
        bool itemInCache = isItemInCache(cache, requests[i]);
        if(!itemInCache) {
            int j =0;
            int k = i;
            int farthestPosition = -1;
            int cacheElementIndex = -1;
            while(j<cache.size()) {
                bool itemExists = true;
                while(k<requests.size()) {
                    if(cache[j] == requests[k] ) {
                        if(k>farthestPosition) {
                            farthestPosition = k;
                            cacheElementIndex = j;
                            break;
                        }
                    } else if(k == (requests.size()-1))
                        itemExists = false;
                    k++;
                }
                if(!itemExists) {
                    cacheElementIndex = j;
                    break;
                }
                j++;   
            }
            evictions.push_back(cache[cacheElementIndex]);
            cache[cacheElementIndex] = requests[i];
        } 
        i++;
    }
    return evictions;
}

int main() {
    vector<int> requests = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };
    int cacheSize = 4;
    vector<int> evictionList = evictions(requests, cacheSize);
    cout<<evictionList.size()<<endl;
    cout<<"Items evicted - ";
    for(int i = 0; i<evictionList.size();i++)
        cout<<evictionList[i]<<",";

    cout<<endl;
    return 0;
    
}