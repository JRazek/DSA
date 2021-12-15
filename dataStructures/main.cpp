#include "SegmentTree.h"
#include <bits/stdc++.h>
#include <random>


int main(){
    using namespace std;
    using Sgtree = sgNs::SumSegmentTree<int>;
    using Node = Sgtree::Node;
    using Range = Sgtree::Range;
    constexpr int N=1000;

    auto summator = [](Range r, std::vector<Node>& nodes){
        int sum =0;
        for(int i=r.low;i<=r.high;i++){
            sum+=nodes[i].numericVal;
        }
        return sum;
    };
    auto rangeAdd = [](Range r, std::vector<Node>& nodes, int val) -> void {
        for(int i=r.low;i<=r.high;i++){
            nodes[i].numericVal+=val;
        }
    };
    
    
    std::vector<Node> nodes(8);
    for(int i = 0; i<nodes.size(); i++){
        nodes[i]={1, nullptr};
    }
    
    Sgtree segmentTree(nodes);

    Range r={0, 6};
    segmentTree.lazyAdd(r, 1);
    rangeAdd(r, nodes, 1);

    Range rq={0, 7};
    int sum1 = segmentTree.rangeQuery(rq);
    int sum2 = summator(rq, nodes);

    std::cout<<"";
    // std::vector<Node> nodes(N);
    // for(int i=0; i<N; i++){
    //     nodes[i]={i, nullptr};
    // }
    // std::shuffle(nodes.begin(), nodes.end(), std::random_device());

    // Sgtree segmentTree(nodes);

    // for(int i=0;i<10000;i++){
    //     Range qr{rand()%nodes.size(), rand()%nodes.size()};
    //     Range lr{rand()%nodes.size(), rand()%nodes.size()};

    //     if(qr.low>qr.high)
    //         std::swap(qr.low, qr.high);
    //     if(lr.low>lr.high)
    //         std::swap(lr.low, lr.high);

    //     int rv=rand()%10*(rand()%2?-1:1);
    //     segmentTree.lazyAdd(lr, rv);
    //     rangeAdd(lr, nodes, rv);

    //     int sum1 = segmentTree.rangeQuery(qr);
    //     int sum2 = summator(qr, nodes);
    //     if(sum1!=sum2){
    //         std::cout<<"NOTOK!!\n";
    //     }
    // }
    
    return 0;
}