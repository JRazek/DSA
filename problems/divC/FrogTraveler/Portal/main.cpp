#include "../../../../dataStructures/prefixSum2D.h"
#include <bits/stdc++.h>
using namespace std;

unsigned long long sum(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, vector<vector<int>>& arr){
    unsigned long long res = 0;
    for(int i = x0; i <= x1; i++){
        for(int j = y0; j <= y1; j++){
            res += arr[i][j];
        }
    }
    return res;
}

int main(){
    const int N = 100;
    const int M = 100;
    const int MAX_NUM = 100;
    PrefixSUM2D<long long> prefixSUM2D(N, M);
    vector<vector<int>> test(N, vector<int>(M));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            int num = rand() % MAX_NUM;
            test[i][j] = num;
            prefixSUM2D.setCell(i, j, num);
        }
    }
    prefixSUM2D.precompute();
    for(int i = 0; i < 1; i++){
        uint32_t x0 = 14, y0 = 45, x1 = 89, y1 = 56;
        unsigned long long r1 = prefixSUM2D.rangeSum(x0, y0, x1, y1);
        unsigned long long r2 = sum(x0, y0, x1, y1, test);
        cout<<(r1 == r2)<<"\n";
    }
    // int sum = prefixSUM2D.rangeSum(8, 8, 9, 9);
}