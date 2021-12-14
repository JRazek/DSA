#include <vector>
#include <cmath>
#include <cstdint>
#include <list>
#include <iostream>

namespace sgNs{
    enum class SGMODE{
        SUM,
        XOR,
        MIN,
        MAX,
    };
    template<
        SGMODE Mode = SGMODE::SUM,
        typename T = u_int64_t,
        typename Y = void*,
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
    private:
        std::vector<Node> nodes;
        u_int realSize;

        void preprocess(){
            u_int fdID=firstFloorID();
            for(int i = firstFloorID()-1; i>=0; i--){
                if constexpr(Mode == SGMODE::SUM){
                    nodes[i].numericVal = nodes[childID(i, true)].numericVal+nodes[childID(i, false)].numericVal;
                }else if constexpr(Mode == SGMODE::MIN){
                    
                }else if constexpr(Mode == SGMODE::MAX){

                }
            }
            std::cout<<"";
        }
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

            left.insert(left.end(), right.begin(), right.end());

            return left;
        }

    public:
        SegmentTree(const std::vector<Node>& _data):
        nodes(std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1),
        realSize(_data.size())
        {
            std::copy(_data.begin(), _data.end(), nodes.begin()+firstFloorID());
            preprocess();
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
        T rangeQuery(Range r){
            std::list<u_int> q=rangeQuery(r, 0);

            if constexpr(Mode == SGMODE::SUM){
                
            }else if constexpr(Mode == SGMODE::MIN){

            }else if constexpr(Mode == SGMODE::MAX){

            }
            return T();
        }
    };
}