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

    public:
        SegmentTreeBase(const std::vector<Node>& _data):
        nodes(std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1, {0, Y()}),
        realSize(_data.size())
        {
            std::copy(_data.begin(), _data.end(), nodes.begin()+firstFloorID());
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
    };

    
    template<
        typename T = int64_t,
        typename Y = void*
    >
    class SumSegmentTree : public SegmentTreeBase<T, Y> {
        using Base = SegmentTreeBase<T, Y>;
        std::vector<T> lazyValues;
        void lazyProp(u_int parent){
            typename Base::Range nr=Base::nodeRange(parent);
            Base::nodes[parent].numericVal+=lazyValues[parent]*(nr.high-nr.low+1);
            lazyValues[Base::childID(parent, true)]+=lazyValues[parent];
            lazyValues[Base::childID(parent, false)]+=lazyValues[parent];
            lazyValues[parent]=0;
        }
    public:
        SumSegmentTree(const std::vector<typename Base::Node>& _data):
        Base(_data),
        lazyValues(Base::nodes.size()){
            u_int fdID=Base::firstFloorID();
            for(int i = Base::firstFloorID()-1; i>=0; i--){
                const T left=Base::nodes[Base::childID(i, true)].numericVal;
                const T right=Base::nodes[Base::childID(i, false)].numericVal;
                Base::nodes[i].numericVal = left+right;
            }
        }
        std::list<u_int> rangeQuery(typename Base::Range r, u_int parent, T lazyTail=T()) {
            typename Base::Range nr=Base::nodeRange(parent);
            r=r.common(nr);
            if(r.low > r.high)
                return {};
                
            lazyProp(parent);
            if(nr==r){
                lazyValues[parent]+=lazyTail;
                return {parent};
            }else{
                Base::nodes[parent].numericVal+=lazyTail*(r.high-r.low+1);
            }
            std::list<u_int> left = rangeQuery(r, Base::childID(parent, true), lazyTail);
            std::list<u_int> right = rangeQuery(r, Base::childID(parent, false), lazyTail);

            left.splice(left.begin(), right);

            return left;
        }
        T rangeQuery(typename Base::Range r){
            const auto l=rangeQuery(r, 0, 0);
            std::cout<<"";
            return std::accumulate(l.begin(), l.end(), T(), [&](const auto s, const auto i2){
                return s+Base::nodes[i2].numericVal;
            });
        }
        void lazyAdd(typename Base::Range r, const T v){
            rangeQuery(r, 0, v);
        }
    };

        
    template<
        typename T = int64_t,
        typename Y = void*,
        typename AssocFunc = std::greater<T>
    >
    class AssocSegmentTree : public SegmentTreeBase<T, Y> {
        using Base = SegmentTreeBase<T, Y>;
    protected:
        AssocFunc assocFunc;
        std::list<u_int> rangeQuery(typename Base::Range r, u_int parent){
            typename Base::Range nr=Base::nodeRange(parent);
            r=r.common(nr);
            if(r.low > r.high)
                return {};
                
            if(nr==r){
                return {parent};
            }

            std::list<u_int> left = rangeQuery(r, Base::childID(parent, true));
            std::list<u_int> right = rangeQuery(r, Base::childID(parent, false));

            left.splice(left.begin(), right);

            return left;
        }
    public:
        template<
            typename AssocFuncT = AssocFunc,
            typename = typename std::enable_if<std::is_default_constructible<AssocFuncT>::value>::type 
        >
        AssocSegmentTree(const std::vector<typename Base::Node>& _data):
        AssocSegmentTree(_data, AssocFunc()){}

        AssocSegmentTree(const std::vector<typename Base::Node>& _data, const AssocFunc& _assocFunc):
        Base(_data),
        assocFunc(_assocFunc){
            for(int i = Base::firstFloorID()-1; i>=0; i--){
                u_int left=Base::childID(i, true);
                u_int right=Base::childID(i, false);
                Base::nodes[i] = (assocFunc(Base::nodes[left].numericVal, Base::nodes[right].numericVal)?Base::nodes[left]:Base::nodes[right]);
            }
        }
        typename Base::Node rangeQuery(typename Base::Range r){
            const auto l=rangeQuery(r, 0);

            return Base::nodes[std::accumulate(l.begin(), l.end(), l.front(), [&](const auto s, const auto i2){
                return (assocFunc(Base::nodes[s].numericVal, Base::nodes[i2].numericVal)?s:i2);
            })];
        }
        void update(u_int leaf, T v){
            u_int id=Base::firstFloorID()+leaf;
            Base::nodes[id].numericVal=v;
            u_int h=Base::height();
            u_int left=Base::childID(id, true);
            u_int right=Base::childID(id, false);
            for(int i=0;i<h;i++){
                Base::nodes[id] = (assocFunc(Base::nodes[left].numericVal, Base::nodes[right].numericVal)?Base::nodes[left]:Base::nodes[right]);
                id=Base::parentID(id);
            }
        }
    };
}
#endif