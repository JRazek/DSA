#include "../dataStructures/SegmentTree.h"
#include <bits/stdc++.h>
#include <random>


int main(){
    using namespace std;
    using namespace sgjr;

    auto assoc=[](int a, int b){
        return std::max(a, b);
    };

    using SegmentTree = SegmentTree<int, minimum<int>>;
    using Range = SegmentTree::Range;
    constexpr int N=1e6;

    auto summator=[](Range r, std::vector<int>& values) -> int{
        int sum=0;
        for(int i=r.low;i<=r.high;i++){
            sum+=values[i];
        }
        return sum;
    };
    auto rangeAdd=[](Range r, std::vector<int>& values, int val) -> void {
        for(int i=r.low;i<=r.high;i++){
            values[i]+=val;
        }
    };
    auto rangeMin=[](Range r, std::vector<int>& values) -> int {
        int min=values[r.low];
        for(int i=r.low;i<=r.high;i++){
            min=std::min(min, values[i]);
        }
        return min;
    };
    
    std::vector<int> values(4, 0);
    SegmentTree tree(values);


    for(int i=0;i<N;i++){
        Range lr{rand()%values.size(), rand()%values.size()};
        Range qr{rand()%values.size(), rand()%values.size()};

        if(qr.low>qr.high)
            std::swap(qr.low, qr.high);
        if(lr.low>lr.high)
            std::swap(lr.low, lr.high);

        int rv=rand()%10*(rand()%2?-1:1);
        tree.lazyAdd(lr, rv);
        rangeAdd(lr, values, rv);

        int sum1 = tree.sumQuery(qr);
        int sum2 = summator(qr, values);
        for(int i=0;i<values.size();i++){
            tree.sumQuery({i,i});
        }
        int min1 = tree.assocQuery(qr);
        int min2 = rangeMin(qr, values);
        if(sum1!=sum2 || min1!=min2){
            std::cout<<"NOT OK!!\n";
        }
    }

    return 0;
}