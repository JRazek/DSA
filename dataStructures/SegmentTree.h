#include <vector>
#include <cmath>
#include <cstdint>


template<
typename T = unsigned long long,
typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class SegmentTree{
    std::vector<T> data;
    int realSize;
    void precompute(){
        for(auto i = 0; i < data.size(); i ++){
            
        }
    }
public:
    SegmentTree(const std::vector<T>& _data){
        realSize = std::pow(2, (std::ceil(std::log2(data.size())) + 1)) - 1;
        data = std::vector<T>(realSize - _data.size());
        data.insert(_data.begin(), _data.end(), data.begin());
    }
    SegmentTree(const SegmentTree& SegmentTree) = default;
    SegmentTree(SegmentTree&& SegmentTree) = default;
    int getChildID(int parentID, bool left){
        return (parentID + 1)*2 + !left - 1;
    }
    int getParentID(int childID){
        return (childID - 1)/2;
    }
    T rangeSum(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) const{
        x0++;
        y0++;
        x1++;
        y1++;
        return data[x1][y1] - data[x0-1][y1] - data[x1][y0-1] + data[x0-1][y0-1];
    }
};