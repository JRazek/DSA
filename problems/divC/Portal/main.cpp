#include <bits/stdc++.h>

#include <vector>
#include <cstdint>
#define INF 100000000

template<
typename T = unsigned long long,
typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class PrefixSUM2D{
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
using namespace std;

unsigned long long sum(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, vector<vector<int>>& arr){
    unsigned long long res = 0;
    for(int i = x0; i <= x1; i++){
        for(int j = y0; j <= y1; j++){
            res += arr[i][j];
        }
    }
    return res;
}

int main(){
    int testCases;
    cin >> testCases;
    for(int testCase = 0; testCase < testCases; testCase++){
        int rows, cols;
        cin >> rows >> cols;
        PrefixSUM2D<> prefixSUM2D(cols, rows);
        for(int i = 0; i < rows; i ++){
            string r;
            cin >> r;
            for(int j = 0; j < cols; j ++){
                prefixSUM2D.setCell(j, i, r[j]-'0');
            }
        }
        prefixSUM2D.precompute();
        int best = INF;
        for(int y = 0; y < rows-4; y ++){
            for(int x = 0; x < cols-3; x ++){
                int inside = prefixSUM2D.rangeSum(x+1, y+1, x+2, y+2);
                int outside = (prefixSUM2D.rangeSum(x, y, x+3, y+3) - inside);
                int res = 14-outside+inside;
                best = min(res, best);
            }
        }
        cout<<best<<"\n";
    }
}