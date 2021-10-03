#include<iostream>
#include<vector>
#include<utility>
#include<cmath>

using namespace std;

void merge(vector<pair<int,int>> &array, int low, int mid, int high, int option) {
    int leftArraySize = mid-low+1;
    int rightArraySize = high -mid;
    string item;

    vector<pair<int,int>> rightArray, leftArray;
    
    for(int i =0;i<rightArraySize;i++)
        rightArray.push_back(array[mid+1+i]);
    
    for(int i=0;i<leftArraySize;i++)
        leftArray.push_back(array[low+i]);

    int leftArrayIterator = 0;
    int rightArrayIterator = 0;
    int indexOfMergedArray = low;
    while(leftArrayIterator<leftArraySize && rightArrayIterator < rightArraySize) {
        if(option == 1) {
            if(leftArray[leftArrayIterator].first <= rightArray[rightArrayIterator].first) {
                array[indexOfMergedArray] = leftArray[leftArrayIterator];
                leftArrayIterator++;
            } else {
                array[indexOfMergedArray] = rightArray[rightArrayIterator];
                rightArrayIterator++;
            }
        } else {
            if(leftArray[leftArrayIterator].second <= rightArray[rightArrayIterator].second) {
                array[indexOfMergedArray] = leftArray[leftArrayIterator];
                leftArrayIterator++;
            } else {
                array[indexOfMergedArray] = rightArray[rightArrayIterator];
                rightArrayIterator++;
            }
        }
        indexOfMergedArray++;
    }

    while(leftArrayIterator<leftArraySize) {
        array[indexOfMergedArray] = leftArray[leftArrayIterator];
        leftArrayIterator++;
        indexOfMergedArray++;
    }

    while(rightArrayIterator<rightArraySize) {
        array[indexOfMergedArray] = rightArray[rightArrayIterator];
        rightArrayIterator++;
        indexOfMergedArray++;
    }
}

/* Sort all points by a given axis (option represents which axis)*/
void mergeSort(vector<pair<int,int>> &array, int low, int high, int option) {
    if(low>=high)
        return;

    int mid = (low+high)/2;
    mergeSort(array, low,mid,1);
    mergeSort(array, mid+1, high,1);
    merge(array,low,mid,high, option);
}

/*Calculate the Euclidean distance between 2 points */
double distanceBetweenPoints (pair<int,int> firstPoint, pair<int, int> secondPoint) {
    return sqrt(pow((firstPoint.first - secondPoint.first),2) + pow((firstPoint.second - secondPoint.second),2));
}

/* Find the closest pair of points recursively */
pair<pair<int,int>, pair<int,int>> recursivelyFindClosestPair(vector<pair<int,int>> pointsSortedByX, vector<pair<int,int>> pointsSortedByY) {
    pair<pair<int,int>, pair<int,int>> closestPairOfPoints;
    
    //Base case, if there are 3 points, compare each pair and find the hortest pair
    if(pointsSortedByX.size() <=3) {
        double minDistance = 10000000000;
        for(int i=0;i<3;i++){
            for(int j=0;j<2;j++) {
                double distance = distanceBetweenPoints(pointsSortedByX[i], pointsSortedByX[(i+1)%3]);
                if(distance<minDistance)
                    minDistance = distance;
                    closestPairOfPoints = {pointsSortedByX[i], pointsSortedByX[(i+1)%3]};
            }
        }
        return closestPairOfPoints;
    }

    int mid = pointsSortedByX.size()/2;
    vector<pair<int,int>> leftSidePointsSortedByX;
    vector<pair<int,int>> leftSidePointsSortedByY;
    vector<pair<int,int>> rightSidePointsSortedByX;
    vector<pair<int,int>> rightSidePointsSortedByY;

    // Create the left and right side of points sorted by x and y each
    for(int i =0; i<=mid;i++)
        leftSidePointsSortedByX.push_back(pointsSortedByX[i]);

    for(int i = mid+1;i<pointsSortedByX.size();i++)
        rightSidePointsSortedByX.push_back(pointsSortedByX[i]);

    for(int i=0;i<pointsSortedByY.size();i++) {
        pair <int,int> thisPoint = pointsSortedByY[i];
        if(thisPoint.first < pointsSortedByX[mid].first)
            leftSidePointsSortedByY.push_back(thisPoint);
        else
            rightSidePointsSortedByY.push_back(thisPoint);
    }

    //Recursively find the closest pair of points on the right and left side
    pair<pair<int,int>, pair<int,int>> leftSideClosestPair = recursivelyFindClosestPair(leftSidePointsSortedByX, leftSidePointsSortedByY);
    pair<pair<int,int>, pair<int,int>> rightSideClosestPair = recursivelyFindClosestPair(rightSidePointsSortedByX, rightSidePointsSortedByY);

    //Calculate delta, the smaller of the shortest distance on the left and right sides
    double leftSideClosestDistance = distanceBetweenPoints(leftSideClosestPair.first, leftSideClosestPair.second);
    double rightSideClosestDistance = distanceBetweenPoints(rightSideClosestPair.first, rightSideClosestPair.second);
    double delta = min(leftSideClosestDistance, rightSideClosestDistance);
    closestPairOfPoints = delta == leftSideClosestDistance? leftSideClosestPair: rightSideClosestPair;


    vector<pair<int, int>> pointsWithinDeltaOfVerticalLineDivider;
    for(int i =0;i< pointsSortedByY.size();i++)
        if(abs(pointsSortedByX[mid].first - pointsSortedByY[i].first) < delta)
            pointsWithinDeltaOfVerticalLineDivider.push_back(pointsSortedByY[i]);

    int windowForMinDistance = 15;

    //Compare with next 15 points, the shortest distance must be between them
    for(int i =0;i<pointsWithinDeltaOfVerticalLineDivider.size();i++) {
        int loopingTerminator;
        if(i+15 < pointsWithinDeltaOfVerticalLineDivider.size())
            loopingTerminator = 16;
        else
            loopingTerminator = pointsWithinDeltaOfVerticalLineDivider.size() - i;

        for(int j=1;j<loopingTerminator;j++)
            if(distanceBetweenPoints(pointsWithinDeltaOfVerticalLineDivider[i], pointsWithinDeltaOfVerticalLineDivider[i+j])< delta) {
                closestPairOfPoints = {pointsWithinDeltaOfVerticalLineDivider[i], pointsWithinDeltaOfVerticalLineDivider[i+j]};
                delta = distanceBetweenPoints(pointsWithinDeltaOfVerticalLineDivider[i], pointsWithinDeltaOfVerticalLineDivider[i+j]);
            } 
    }

    cout <<"Smallest distance - "<<delta<<endl;
    return closestPairOfPoints;

}

pair<pair<int, int>, pair<int, int>> closestPair(vector<pair<int,int>> points) {
    vector<pair<int,int>> pairsSortedByX ;
    vector<pair<int,int>> pairsSortedByY ;
    for(int i=0; i<points.size(); i++){
        pairsSortedByX.push_back(points[i]);
        pairsSortedByY.push_back(points[i]);
    }

    mergeSort(pairsSortedByX, 0, pairsSortedByX.size()-1,1);
    mergeSort(pairsSortedByY, 0, pairsSortedByY.size()-1,2);
    return recursivelyFindClosestPair(pairsSortedByX, pairsSortedByY);
}

int main() {
    vector<pair<int,int>> points = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    pair<pair<int,int>, pair<int,int>> closestPairOfPoints = closestPair(points);
    cout<<"Closest Pair Of Points - ("<<closestPairOfPoints.first.first<<","<<closestPairOfPoints.first.second<<"),("<<closestPairOfPoints.second.first<<","<<closestPairOfPoints.second.second<<")"<<endl;
    return 0;
}