#include "SegmentTree.h"
#include <bits/stdc++.h>


int main(){
    using Sgtree = sgNs::SumSegmentTree<int>;
    using Node = Sgtree::Node;
    using Range = Sgtree::Range;
    Node node;
    Sgtree segmentTree(std::vector<Node>{node, node, node, node, node});
    // Range r=segmentTree.nodeRange(6);

    // segmentTree.rangeQuery({0, 2});
    return 0;
}