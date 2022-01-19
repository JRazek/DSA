#ifndef SEGMENT_TREE_DSJR
#define SEGMENT_TREE_DSJR

#include <vector>
#include <cmath>
#include <cstdint>
#include <list>
#include <iostream>
#include <numeric>
#include <algorithm>

namespace sgjr{
    //O(logn) query time
    //O(2N) space
    //O(N) preprocess

    template<typename T>
    struct minimum{
        auto operator()(const T& a, const T& b) const noexcept -> T
        { return (a<b?a:b); }
    };
    template<typename T>
    struct maximum{
        auto operator()(const T& a, const T& b) const noexcept -> T
        { return (a>b?a:b); }
    };

    template<
        typename T = int64_t,
        typename Y = void*,
        typename AssocFunc = maximum<T>,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SegmentTree{
    public:
        struct Range{
            int low;
            int high;
            Range common(Range rhs) const{
                return {std::max(low, rhs.low), std::min(high, rhs.high)};
            }
            bool operator==(const Range& r){
                return low==r.low&&high==r.high;
            };
            bool operator!=(const Range& r){
                return !(*this==r);
            };
        };
    protected:
    
        int realSize;
        struct Node{
            T value=T();
            T asValue=T();
            T lazy=T();
        };
        std::vector<Node> nodes;
        AssocFunc assocFunc;
        
        auto rangeQuery(Range qr, int nodeID) -> std::list<int> {
            if(nodeID>=nodes.size()) return {};

            auto nr=nodeRange(nodeID);
            qr=qr.common(nr);
            if(qr.low>qr.high) return {};
            
            auto& node=nodes[nodeID];
            std::list<int> result;


            int leftID=childID(nodeID, true);
            int rightID=childID(nodeID, false);

            //it will only go one level deeper due to full overlap condition.
            lazyUpdate({nr.low, (nr.low+nr.high)/2}, leftID, node.lazy);
            lazyUpdate({(nr.low+nr.high)/2+1, nr.high}, rightID, node.lazy);
            node.lazy=T();

            if(qr==nr) result.emplace_back(nodeID); 
            else{
                result.splice(result.end(), rangeQuery(qr, leftID));
                result.splice(result.end(), rangeQuery(qr, rightID));
            }
            node.asValue=assocFunc(nodes[leftID].asValue, nodes[rightID].asValue);
            return result;
        }
        auto lazyUpdate(Range qr, int nodeID, int value) -> void {
            if(nodeID>=nodes.size()) return;
            
            auto nr=nodeRange(nodeID);
            qr=qr.common(nr);
            if(qr.low>qr.high) return;

            auto& node=nodes[nodeID];
            node.value+=value*(qr.high-qr.low+1);
            node.asValue+=value;

            if(qr==nr){
                node.lazy+=value;
            }else{
                lazyUpdate(qr, childID(nodeID, true), value);
                lazyUpdate(qr, childID(nodeID, false), value);
            }
        }
    public:
        SegmentTree(const std::vector<T>& _data):
        realSize(_data.size()),
        nodes(std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1){;

            int fdID=firstFloorID();

            for(int i=0;i<_data.size();i++){
                nodes[fdID+i].value=_data[i];
            }
            for(int i = firstFloorID()-1; i>=0; i--){
                auto leftID=childID(i, true);
                auto rightID=childID(i, false);
                
                auto& node=nodes[i];
                node.value=nodes[leftID].value+nodes[rightID].value;
                node.asValue=assocFunc(nodes[leftID].value, nodes[rightID].value);
            }
        }

        auto height() const -> int {
            return height(nodes.size());
        }
        auto height(int nodeID) const -> int {
            return std::log2(nodeID+1);
        }
        auto firstFloorID() const -> int {
            return std::pow(2, height()-1)-1;
        }
        auto childID(int parentID, bool left) const -> int{
            return (parentID + 1)*2-left;
        }
        auto parentID(int childID) const -> int {
            return (childID - 1)/2;
        }
        auto nodeRange(int nodeID) const -> Range {
            auto nh=height(nodeID);
            auto h=height();
            auto sz=(int)(std::pow(2, h-nh-1));
            auto shift=(int)((nodeID-(std::pow(2, nh)-1))*std::pow(2, h-nh-1));
            return {shift, shift+sz-1};
        }

        auto assocQuery(Range r) -> T {
            const auto l=rangeQuery(r, 0);
            return std::accumulate(l.begin(), l.end(), T(), [&](const auto s, const auto i2){
                return assocFunc(s, nodes[i2].asValue);
            });
        }
        auto sumQuery(Range r) -> T {
            const auto l=rangeQuery(r, 0);
            return std::accumulate(l.begin(), l.end(), T(), [&](const auto s, const auto i2){
                return s+nodes[i2].value;
            });
        }
        /**
         * @brief just add in case of default tree. If OperationFunction is overriden - it will perform whatever u want :)
         * 
         * @param r 
         * @param v 
         */
        void lazyAdd(Range r, const T v){
            lazyUpdate(r, 0, v);
        }
    };
}
#endif //SEGMENT_TREE_DSJR
