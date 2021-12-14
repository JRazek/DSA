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
                return {std::max(high, rhs.high), std::min(low, rhs.low)};
            }
        };
    private:
        std::vector<Node> nodes;
        std::vector<Range> ranges;
        u_int realSize;

        void preprocess(){
            u_int fdID=firstFloorID();
            for(int i = fdID; i<nodes.size(); i++){
                ranges[i].low=i-fdID;
                ranges[i].high=i-fdID;
            }
            for(int i = firstFloorID()-1; i>=0; i--){
                ranges[i] = {ranges[childID(i, true)].low, ranges[childID(i, false)].high};
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
            r=r.common(ranges[parent]);
            if(ranges[parent]==r){
                return {parent};
            }
            std::list<u_int> left = rangeQuery(r, childID(true, parent));
            std::list<u_int> right = rangeQuery(r, childID(false, parent));
            // return std::copy() [todo] join these two in O(1)
        }

    public:
        SegmentTree(const std::vector<Node>& _data):
        nodes(std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1),
        ranges(nodes.size()),
        realSize(_data.size())
        {
            std::copy(_data.begin(), _data.end(), nodes.begin()+firstFloorID());
            preprocess();
        }
        u_int height() const{
            return std::log2(nodes.size()+1);
        }
        u_int firstFloorID() const{
            return std::pow(2, height() - 1) - 1;
        }
        u_int childID(u_int parentID, bool left) const{
            return (parentID + 1)*2 + !left - 1;
        }
        u_int getParentID(u_int childID) const{
            return (childID - 1)/2;
        }

        T rangeQuery(Range r){
            if constexpr(Mode == SGMODE::SUM){
                
            }else if constexpr(Mode == SGMODE::MIN){

            }else if constexpr(Mode == SGMODE::MAX){

            }
        }
    };
}