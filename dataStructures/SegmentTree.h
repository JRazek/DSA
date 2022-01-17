#ifndef SEGMENT_TREE_DSJR
#define SEGMENT_TREE_DSJR

#include <vector>
#include <cmath>
#include <cstdint>
#include <list>
#include <iostream>
#include <numeric>
#include <functional>

namespace sgjr{
    //O(logn) query time
    //O(2N) space
    //O(N) preprocess

    template<
        typename T = int64_t,
        typename Y = void*,
        typename OperationFunc = std::plus<T>,
        typename AssocFunc = std::greater<T>,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SegmentTree{
    public:
        struct Node{
            T numericVal;
            Y data;
        };
        struct Range{
            u_int low;
            u_int high;
            Range common(Range rhs) const{
                return {std::max(low, rhs.low), std::min(high, rhs.high)};
            }
            bool operator==(const Range& r){
                return low==r.low&&high==r.high;
            };
        };
    protected:
        std::vector<Node> nodes;
        u_int realSize;
        std::vector<T> lazyValues;
        OperationFunc operationFunc;
        AssocFunc assocFunc;

        void lazyProp(u_int parent){
            Range nr=nodeRange(parent);
            nodes[parent].numericVal+=lazyValues[parent]*(nr.high-nr.low+1);
            lazyValues[childID(parent, true)]+=lazyValues[parent];
            lazyValues[childID(parent, false)]+=lazyValues[parent];
            lazyValues[parent]=0;
        }
        std::list<u_int> rangeQuery(Range r, u_int parent, T lazyTail=T()) {
            Range nr=nodeRange(parent);
            r=r.common(nr);
            if(r.low > r.high)
                return {};
                
            lazyProp(parent);
            if(nr==r){
                lazyValues[parent]+=lazyTail;
                return {parent};
            }else{
                nodes[parent].numericVal+=lazyTail*(r.high-r.low+1);
            }
            std::list<u_int> left = rangeQuery(r, childID(parent, true), lazyTail);
            std::list<u_int> right = rangeQuery(r, childID(parent, false), lazyTail);

            left.splice(left.begin(), right);

            return left;
        }
    public:

        SegmentTree(const std::vector<Node>& _data):
        nodes(std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1, {0, Y()}),
        realSize(_data.size()),
        lazyValues(nodes.size()) {
            std::copy(_data.begin(), _data.end(), nodes.begin()+firstFloorID());
            u_int fdID=firstFloorID();
            for(int i = firstFloorID()-1; i>=0; i--){
                const T left=nodes[childID(i, true)].numericVal;
                const T right=nodes[childID(i, false)].numericVal;
                nodes[i].numericVal = operationFunc(left, right);
            }
        }

        u_int height() const{
            return height(nodes.size());
        }
        u_int height(u_int nodeID) const{
            return std::log2(nodeID+1);
        }
        u_int firstFloorID() const{
            return std::pow(2, height()-1)-1;
        }
        u_int childID(u_int parentID, bool left) const{
            u_int t=(parentID + 1)*2 + !left - 1;
            return t<nodes.size()?t:parentID;
        }
        u_int parentID(u_int childID) const{
            return (childID - 1)/2;
        }
        Range nodeRange(u_int nodeID) const{
            u_int nh=height(nodeID);
            u_int h=height();
            u_int sz=std::pow(2, h-nh-1);
            u_int shift=(nodeID-((u_int)std::pow(2, nh)-1))*std::pow(2, h-nh-1);
            return {shift, shift+sz-1};
        }
        T rangeQuery(Range r){
            const auto l=rangeQuery(r, 0, 0);
            return std::accumulate(l.begin(), l.end(), T(), [&](const auto s, const auto i2){
                return operationFunc(s, nodes[i2].numericVal);
            });
        }
        void lazyAdd(Range r, const T v){
            rangeQuery(r, 0, v);
        }
    };
}
#endif //SEGMENT_TREE_DSJR
