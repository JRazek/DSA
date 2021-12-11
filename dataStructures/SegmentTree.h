#include <vector>
#include <cmath>
#include <cstdint>

namespace sgNs{
    enum class SGMODE{
        MIN,
        MAX,
        SUM,
    };
    template<
        SGMODE Mode,
        typename Y,
        typename T = u_int64_t,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SegmentTree{
    public:
        struct Node{
            T numericVal;
            Y data;
        };
    private:
        std::vector<Node> data;
        u_int realSize;
    public:
        SegmentTree(const std::vector<Node>& _data){
            realSize = std::pow(2, (std::ceil(std::log2(_data.size())) + 1)) - 1;
            u_int height = std::ceil(std::log2(data.size()))+1;
            int firstFloorID = std::pow(2, height - 1) - 1;
            data = std::vector<Node>(realSize - _data.size());
            std::copy(_data.begin(), _data.end(), data.begin());
        }

        u_int getChildID(u_int parentID, bool left){
            return (parentID + 1)*2 + !left - 1;
        }
        u_int getParentID(u_int childID){
            return (childID - 1)/2;
        }
        std::vector<Node> rangeQuery(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) const{
            if constexpr(Mode == SGMODE::SUM){
                
            }else if constexpr(Mode == SGMODE::MIN){

            }else if constexpr(Mode == SGMODE::MAX){

            }
        }
    };
}