#include <bits/stdc++.h>
#include "SegmentTree.h"
using namespace std;

int main(){
    constexpr int N = 1000;
    std::vector<int> data(N);
    for(auto i = 0 ;  i< N ; i ++){
        data[i] = rand() % N;
    }
    SegmentTree<> segmentTree();
    return 0;
}