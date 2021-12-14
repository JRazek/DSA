#include "SegmentTree.h"

int main(){
    using Sgtree = sgNs::SegmentTree<sgNs::SGMODE::SUM, int>;
    using Node = Sgtree::Node;
    Node node;
    Sgtree segmentTree({node, node, node, node});
    return 0;
}