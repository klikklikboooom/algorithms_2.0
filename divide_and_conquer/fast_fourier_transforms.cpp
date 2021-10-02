#include<iostream>
#include<vector>
#include<complex>
#include<cmath>

using namespace std;

typedef complex<double> cd; 

vector<cd> fft(vector<cd> &polynomial) {
    int size = polynomial.size();
    if(size == 1)
        return vector<cd>{1, polynomial[0]};
    
    vector<cd> complexRootsOfUnity;
    for(int i =0;i<size;i++) {
        double alpha = -2 * M_PI * i / size;
        complexRootsOfUnity.push_back({cd(cos(alpha), sin(alpha))}); 
    }

    vector<cd> evenIndexedPolynomials, oddIndexedPolynomials;
    for(int i =0;i<size/2;i++) {
        evenIndexedPolynomials.push_back(polynomial[i*2]);
        oddIndexedPolynomials.push_back(polynomial[i*2+1]);
    }

    vector<cd> y0 = fft(evenIndexedPolynomials);
    vector<cd> y1 = fft(oddIndexedPolynomials);

    vector<cd> solution(size);
    for(int k =0;k<size/2;k++) {
        solution[k] = y0[k] + complexRootsOfUnity[k]*y1[k];
        solution[k+size/2] = y0[k] - complexRootsOfUnity[k]*y1[k];
    }

    return solution;
}

int main () {
    vector<cd> a = {1,2,3,4};
    vector<cd> b = fft(a);
    for(int i =0;i<b.size();i++)
        cout<<b[i]<<endl;
    return 0;
}