#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

double recursivelyMultiply(int a, int b) {

    if(a<10 || b < 10)
        return a*b;
    int aCopy = a, bCopy = b;
    int aLen = 0;
    int bLen = 0;
    
    while(aCopy > 1){
        aLen++;
        aCopy = aCopy/10;
    }

    while (bCopy > 0) {
        bLen++;
        bCopy = bCopy/10;
    }
    int halfALen = aLen/2;
    int halfBLen = bLen/2;

    int aDivider = pow(10, halfALen);
    int leftHalfOfA = a/aDivider;
    int rightHalfOfA = a%aDivider;
    int bDivider = pow(10, halfBLen);
    int leftHalfOfB = b/bDivider;
    int rightHalfOfB = b%bDivider;

    double intermediateProduct = recursivelyMultiply(leftHalfOfA+rightHalfOfA, leftHalfOfB+rightHalfOfB);
    double leftHalfOfBothProduct = recursivelyMultiply(leftHalfOfA, leftHalfOfB);
    double rightHalfOfBothProduct = recursivelyMultiply(rightHalfOfA, rightHalfOfB);
    
    return (leftHalfOfBothProduct*pow(10,aLen) + (intermediateProduct - leftHalfOfBothProduct - rightHalfOfBothProduct)*pow(10, aLen/2) + rightHalfOfBothProduct);
}

int main() {

    int a = 9876;
    int b = 1234;

    double product = recursivelyMultiply(a,b);
    cout.precision(8);
    cout<<"9876*1234 = "<<product<<endl;
    return 0;
}