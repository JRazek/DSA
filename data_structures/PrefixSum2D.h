#ifndef PREFIX_SUM_DSJR
#define PREFIX_SUM_DSJR

#include <vector>
#include <cstdint>
#include <cmath>

namespace dsjr{
    //O(1) query time
    //O(n) space
    //O(n) preprocess

    template<
        typename T = long long,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class PrefixSUM2D{
    protected:
        uint32_t cols, rows;
        std::vector<std::vector<T>> data;

    public:
        PrefixSUM2D(uint32_t _cols, uint32_t _rows):cols(_cols), rows(_rows), data(cols+1, std::vector<T>(rows+1, 0)){}
        PrefixSUM2D(const PrefixSUM2D& prefixSUM2D) = default;
        PrefixSUM2D(PrefixSUM2D&& prefixSUM2D) = default;

        void setCell(uint32_t x, uint32_t y, const T &val){
            data[x+1][y+1] = val;
        }
        T getCell(uint32_t x, uint32_t y) const{
            return data[x+1][y+1];
        }
        void precompute(){
            for(auto y = 0; y < rows; y++){
                for(auto x = 0; x < cols; x++){
                    int v0=0, v1=0, v2=0, v3=0;
                    if(x>0 && y>0)
                        v0 = rangeSum(0, 0, x-1, y-1);
                    if(x>0)
                        v1 = rangeSum(0, y, x-1, y);
                    if(y>0)
                        v2 = rangeSum(x, 0, x, y-1);
                    int add = rangeSum(0, 0, x-1, y-1) + rangeSum(0, y, x-1, y) + rangeSum(x, 0, x, y-1);
                    data[x+1][y+1] += add;
                }
            }
        }
        T rangeSum(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) const{
            x0++;
            y0++;
            x1++;
            y1++;
            return data[x1][y1] - data[x0-1][y1] - data[x1][y0-1] + data[x0-1][y0-1];
        }
    };

    template<
        typename T = long long,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class PrefixSum1D : protected PrefixSUM2D<T> {
        using Base=PrefixSUM2D<T>;
    public:
        PrefixSum1D(uint32_t size):
        Base(size, 1){}
        PrefixSum1D(const std::vector<T>& _data):
        Base(_data.size(), 1){
            for(int i=0;i<_data.size();i++) Base::setCell(i, 0, _data[i]);
            Base::precompute();
        }
        T rangeSum(uint32_t low, uint32_t high) const{
            return PrefixSUM2D<T>::rangeSum(low, 0, high, 0);
        }
        void setCell(uint32_t x, const T &val){
            Base::setCell(x, 0, val);
        }
    };
}
#endif