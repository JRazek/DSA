#include <vector>
#include <cmath>
#include <cstdint>
#include <list>
#include <iostream>
#include <numeric>

namespace sgNs{
    enum class SGMODE{
        SUM,
        XOR,
        MIN,
        MAX,
    };
    template<
        typename T = u_int64_t,
        typename Y = void*,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SegmentTreeBase{
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

        std::list<u_int> rangeQuery(Range r, u_int parent) const{
            if(r.low > r.high)
                return {};
            Range nr=nodeRange(parent);
            r=r.common(nr);
            if(nr==r){
                return {parent};
            }
            std::list<u_int> left = rangeQuery(r, childID(parent, true));
            std::list<u_int> right = rangeQuery(r, childID(parent, false));

            left.splice(left.end(), right);

            return left;
        }
    public:
        SegmentTreeBase(const std::vector<Node>& _data):
        nodes(std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1),
        realSize(_data.size())
        {
            std::copy(_data.begin(), _data.end(), nodes.begin()+firstFloorID());
        }
        u_int height() const{
            return std::log2(nodes.size()+1);
        }
        u_int height(u_int nodeID) const{
            return std::log2(nodeID+1);
        }
        u_int firstFloorID() const{
            return std::pow(2, height()-1)-1;
        }
        u_int childID(u_int parentID, bool left) const{
            return (parentID + 1)*2 + !left - 1;
        }
        u_int getParentID(u_int childID) const{
            return (childID - 1)/2;
        }
        Range nodeRange(u_int nodeID) const{
            u_int nh=height(nodeID);
            u_int h=height();
            u_int sz=std::pow(2, h-nh-1);
            u_int shift=(nodeID-((u_int)std::pow(2, nh)-1))*std::pow(2, h-nh-1);
            return {shift, shift+sz-1};
        }
    };

    
    template<
        typename T = u_int64_t,
        typename Y = void*
    >
    class SumSegmentTree : public SegmentTreeBase<T, Y> {
        using Base = SegmentTreeBase<T, Y>;
    public:
        SumSegmentTree(const std::vector<typename Base::Node>& _data):
        Base(_data){
            u_int fdID=Base::firstFloorID();
            for(int i = Base::firstFloorID()-1; i>=0; i--){
                const T left=Base::nodes[Base::childID(i, true)].numericVal;
                const T right=Base::nodes[Base::childID(i, false)].numericVal;
                Base::nodes[i].numericVal = left+right;
            }
        }
        T rangeQuery(typename Base::Range r){
            const auto l = Base::rangeQuery(r, 0);
            return std::accumulate(l.begin(), l.end(), T(), [&](const auto s, const auto i2){
                return s+Base::nodes[i2].numericVal;
            });
        }
        void lazyAdd(typename Base::Range r, const T v){
            
        }
    };
    template<
        typename T = u_int64_t,
        typename Y = void*
    >
    class XORSegmentTree : public SegmentTreeBase<T, Y> {
        using Base = SegmentTreeBase<T, Y>;
    public:
        XORSegmentTree(const std::vector<typename Base::Node>& _data):
        Base(_data){
            u_int fdID=Base::firstFloorID();
            for(int i = Base::firstFloorID()-1; i>=0; i--){
                const T left=Base::nodes[Base::childID(i, true)].numericVal;
                const T right=Base::nodes[Base::childID(i, false)].numericVal;
                Base::nodes[i].numericVal = left+right;
            }
        }
        T rangeQuery(typename Base::Range r){
            const auto l = Base::rangeQuery(r, 0);
            return std::accumulate(l.begin(), l.end(), T(), [&](const auto s, const auto i2) {
                return s^Base::nodes[i2].numericVal;
            });
        }
    };
}