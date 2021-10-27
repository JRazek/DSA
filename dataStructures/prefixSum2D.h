#include <vector>
#include <cstdint>


template<
typename T = unsigned long long,
typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class PrefixSUM2D{
    uint32_t n, m;
    std::vector<std::vector<T>> data;
public:
    PrefixSUM2D(uint32_t _n, uint32_t _m):n(_n), m(_m), data(n+1, std::vector<T>(m+1, 0)){}
    PrefixSUM2D(const PrefixSUM2D& prefixSUM2D) = default;
    PrefixSUM2D(PrefixSUM2D&& prefixSUM2D) = default;

    void setCell(uint32_t x, uint32_t y, const T &val){
        data[x+1][y+1] = val;
    }
    T getCell(uint32_t x, uint32_t y) const{
        return data[x+1][y+1];
    }
    void precompute(){
        for(auto y = 0; y < n; y++){
            for(auto x = 0; x < m; x++){
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