#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

pair<vector<vector<int>> , vector<vector<int>>> createSubMatrix(vector<vector<int>> a, vector<vector<int>> b, int startingI, int startingJ) {
    int halfSize = a.size()/2;
    vector<vector<int>> subMatrixOfA(halfSize);
    vector<vector<int>> subMatrixOfB(halfSize);

    for(int i =startingI; i<halfSize+startingI;i++) {
        for(int j = startingJ; j <halfSize+startingJ;j++) {
            subMatrixOfA[i - startingI].push_back(a[i][j]);
            subMatrixOfB[i - startingI].push_back(b[i][j]);
        }
    }

    return {subMatrixOfA, subMatrixOfB};
}

vector<vector<int>> matrixAddition(vector<vector<int>> a, vector<vector<int>> b) {
    vector<vector<int>> sum(a.size());
    for(int i =0;i<a.size();i++)
        for(int j =0;j<a.size();j++)
            sum[i].push_back(a[i][j] + b[i][j]);
    
    return sum;
}

vector<vector<int>> matrixSubtraction(vector<vector<int>> a, vector<vector<int>> b) {
    vector<vector<int>> difference(a.size());
    for(int i =0;i<a.size();i++)
        for(int j =0;j<a.size();j++)
            difference[i].push_back(a[i][j] - b[i][j]);
    
    return difference;
}

vector<vector<int>> strassensMultiplication(vector<vector<int>> a, vector<vector<int>> b) {
    if (a.size() == 1 && a[0].size() == 1)
        return {{a[0][0] *b[0][0]}};

    vector<pair<vector<vector<int>>, vector<vector<int>>>> subMatrices;
    int aHalfLen = a.size()/2;
    for(int i=0;i<a.size();i+=aHalfLen){
        for(int j=0;j<a.size();j+=aHalfLen) {
            pair<vector<vector<int>>, vector<vector<int>>> subMatrix = createSubMatrix(a,b,i,j);
            subMatrices.push_back(subMatrix);
        }
    }

    vector<vector<int>> p1Matrix = strassensMultiplication(subMatrices[0].first, matrixSubtraction(subMatrices[1].second, subMatrices[3].second));
    vector<vector<int>> p2Matrix = strassensMultiplication(matrixAddition(subMatrices[0].first, subMatrices[1].first), subMatrices[3].second);
    vector<vector<int>> p3Matrix = strassensMultiplication(matrixAddition(subMatrices[2].first, subMatrices[3].first), subMatrices[0].second);
    vector<vector<int>> p4Matrix = strassensMultiplication(subMatrices[3].first, matrixSubtraction(subMatrices[2].second, subMatrices[0].second));
    vector<vector<int>> p5Matrix = strassensMultiplication(matrixAddition(subMatrices[0].first, subMatrices[3].first), matrixAddition(subMatrices[0].second, subMatrices[3].second));
    vector<vector<int>> p6Matrix = strassensMultiplication(matrixSubtraction(subMatrices[1].first, subMatrices[3].first), matrixSubtraction(subMatrices[2].second, subMatrices[3].second));
    vector<vector<int>> p7Matrix = strassensMultiplication(matrixSubtraction(subMatrices[0].first, subMatrices[2].first), matrixAddition(subMatrices[0].second, subMatrices[1].second));
    
    vector<vector<int>> productMatrix(2, vector<int>(2));
    productMatrix[0][0] = p5Matrix[0][0] + p4Matrix[0][0] - p2Matrix[0][0] + p6Matrix[0][0];
    productMatrix[0][1] = p1Matrix[0][0] + p2Matrix[0][0];
    productMatrix[1][0] = p3Matrix[0][0] + p4Matrix[0][0];
    productMatrix[1][1] = p1Matrix[0][0] + p5Matrix[0][0] - p3Matrix[0][0] - p7Matrix[0][0];

    return productMatrix;
}

int main() {
    vector<vector<int>> matrixA = {{5,7,9,10}, {2,3,3,8}, {8,10,2,3}, {3,3,4,8}};
    vector<vector<int>> matrixB = {{3,10,12,18}, {12,1,4,9}, {9,10,12,2}, {3,12,4,10}};

    vector<vector<int>> productMatrix = strassensMultiplication(matrixA, matrixB);
    cout<<"Product"<<endl;

    for(int i = 0; i<productMatrix.size();i++) {
        for(int j = 0; j < productMatrix[0].size();j++) {
            cout<<productMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}