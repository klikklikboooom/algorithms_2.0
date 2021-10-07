#include<iostream>
#include<vector>
#include<cmath>
#include <stack>
#define INF numeric_limits<double>::infinity()

using namespace std;

typedef struct point {
    int x,y;
} point;

/* Find the partition, i.e, where the pivot element should be in the array when performing quicksort */
int partition(vector<point> &arrToSort, int low, int high) {
    int pivot = arrToSort[high].x;
    int i = low -1;
    for(int j = low; j<high;j++) {
        if(arrToSort[j].x<pivot) {
            i++;
            swap(arrToSort[i], arrToSort[j]);
        }
    }
    swap(arrToSort[i+1], arrToSort[high]);
    return i+1;
}

/* Quicksort to sort jobs by finish time */
void sortPointsByX(vector<point> &points, int low, int high) {
    if(low<high) {
        int p = partition(points, low, high);
        sortPointsByX(points, low, p-1);
        sortPointsByX(points, p+1, high); 
    }   
}

vector<vector<double>> calculateErrorValues(vector<point> points, int newLineCost) {
    int num = points.size();
    sortPointsByX(points, 0, points.size()-1);
    vector<int> cumulativeXY, cumulativeY, cumulativeX, cumulativeXSquared;
    vector<vector<double>> errors(num , vector<double>(num)); 

    for(int j = 0; j< points.size();j++) {
        if(j == 0) {
            cumulativeX.push_back(points[j].x);
            cumulativeY.push_back(points[j].y);
            cumulativeXY.push_back(points[j].x*points[j].y);
            cumulativeXSquared.push_back(points[j].x*points[j].x);
        } else {
            cumulativeX.push_back(points[j].x + cumulativeX[j-1]);
            cumulativeY.push_back(points[j].y + cumulativeY[j-1]);
            cumulativeXY.push_back(points[j].x*points[j].y + cumulativeXY[j-1]);
            cumulativeXSquared.push_back(points[j].x*points[j].x + cumulativeXSquared[j-1]);
        }
        for(int i =0;i<=j;i++) {
            int interval = j-i+1;
            int xSum = cumulativeX[j] - cumulativeX[i-1];
            int ySum = cumulativeY[j] - cumulativeY[i-1];
            int xySum = cumulativeXY[j] - cumulativeXY[i-1];
            int xSquaredSum = cumulativeXSquared[j] - cumulativeXSquared[i-1];

            double thisSlopeDenom = (interval*xSquaredSum) - (xSum*xSum);
            double thisSlope = thisSlopeDenom == 0? INF: (double)  ((interval*xySum) - (xSum*ySum))/ thisSlopeDenom;
            double thisIntercept = (ySum - thisSlope*xSum)/interval;

            for(int k = i;k<=j;k++) {
                double thisError = pow(points[k].y - (thisSlope*points[k].x) - thisIntercept,2);
                errors[i][j] = thisError;
            }
        }
    }

    return errors;
}

int leastSegmentedSquares(vector<point> points, int newLineCost) {
    int num = points.size();
    vector<double> optimalCost;
    vector<double> optimalSegment;
    stack<int> segments;

    vector<vector<double>> errors = calculateErrorValues(points, newLineCost);
    for(int j = 0; j< points.size();j++) {
        double minError = INF;
        int optimalIndex = 0;
        for(int i =0;i<=j;i++) {
            double tempOpt;
            if(i == 0)
                tempOpt = errors[i][j];
            else {
                tempOpt = errors[i][j] + optimalCost[i-1];
                if(tempOpt < minError) {
                    minError = tempOpt;
                    optimalIndex = i;
                }
            }
            optimalCost.push_back(minError+newLineCost);
            optimalSegment.push_back(optimalIndex);
        }
    }

    for (int i = num-1 , j = optimalSegment[num-1]; i >= 0; i = j-1, j = optimalSegment[i])	{
		segments.push(i);
		segments.push(j);
	}

    cout<<"Segments"<<endl;
    int segementNumber = 1;
    while(!segments.empty()) {
        int i = segments.top(); 
        segments.pop();
		int j = segments.top(); segments.pop();
        segments.pop();
        cout<<"Segment number "<<segementNumber<<" : "<<i<< " - "<<j;
    }
}

int main() {

    return 0;
}