#include "../../../../dataStructures/prefixSum2D.h"
#include <bits/stdc++.h>
using namespace std;
int main(){
    PrefixSUM2D<long long> prefixSUM2D(100, 100);
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 100; j++){
            prefixSUM2D.setCell(i, j, 1);
        }
    }
    prefixSUM2D.precompute();
    int sum = prefixSUM2D.rangeSum(8, 8, 9, 9);
    cout<<sum<<"\n";
}