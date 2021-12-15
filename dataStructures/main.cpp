#include "SegmentTree.h"
#include <bits/stdc++.h>


int main(){
    using namespace std;
    using Sgtree = sgNs::SumSegmentTree<int>;
    using Node = Sgtree::Node;
    using Range = Sgtree::Range;
    Node node{0, nullptr};
    Sgtree segmentTree(std::vector<Node>{node, node, node, node, node});


    // Range r=segmentTree.nodeRange(6);

    int sum = segmentTree.rangeQuery({0, 2});
    cout<<sum<<"\n";
    return 0;
}