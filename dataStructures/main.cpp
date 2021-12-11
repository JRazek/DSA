#include "SegmentTree.h"

int main(){
    using Sgtree = sgNs::SegmentTree<sgNs::SGMODE::SUM, void*, int>;
    using Node = Sgtree::Node;
    Node node;
    Sgtree segmentTree({node});
    return 0;
}