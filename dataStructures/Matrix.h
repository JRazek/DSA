#include <iostream>
#include <array>
#include <bitset>
#include <numeric>
#include <algorithm>

namespace jr{
template<
  u_int32_t N,
  u_int32_t M,
  typename T
>
struct Matrix{
    std::array<std::array<T, M>, N> a;
    
    Matrix():
    a({}){};
    Matrix(std::array<std::array<T, M>, N> const& arr):
    a(arr){}

    template<
        u_int32_t X
    >
    auto operator*(Matrix<M, X, T> const& rhs) const -> Matrix<N, X, T>{
        Matrix<N, X, T> res;
        multiply(*this, rhs, res);
        return res;
    }
    auto operator*(const T scalar) const -> Matrix{
        auto res=*this;
        multiply(res, scalar);
        return res;
    }
    auto operator*=(Matrix<M, M, T> const& rhs) -> Matrix{
        multiply(*this, rhs, *this);
        return *this;
    }
    auto operator*=(const T scalar) -> Matrix&{
        multiply(*this, scalar);
        return *this;
    }

    auto operator+(Matrix const& rhs) const -> Matrix{
        auto res=*this;
        add(*this, rhs, res);
        return res;
    }
    auto operator+=(Matrix const& rhs) -> Matrix&{
        add(*this, rhs, *this);
        return *this;
    }
    auto operator-(Matrix const& rhs) const -> Matrix{
        auto res=*this;
        subtract(*this, rhs, res);
        return res;
    }
    auto operator-=(Matrix const& rhs) -> Matrix&{
        subtract(*this, rhs, *this);
        return *this;
    }
    
    auto operator[](const u_int32_t y) const -> std::array<T, M> const&{
        return a[y];
    }
    auto operator[](const u_int32_t y) -> std::array<T, M>&{
        return a[y];
    }

    //FIXME - implement La' Places algorithm
    auto determinant() const requires(M==N && M<=3){
        auto det=T();
        for(auto x=0;x<N;x++){
            auto tmp1=T(1);
            auto tmp2=T(1);
            for(auto y=0;y<N;y++){
                tmp1*=a[y][(x+y)%M];
                tmp2*=a[y][(N-1+x-y)%M];
            }
            det+=tmp1-tmp2;
        }
        return det;
    }
    auto gauss_elimination(){
        auto& mat=*this;
        sort_matrix(mat);
        row_echelon(mat);

        std::cout<<mat<<'\n';
    }
    auto element_wise_xor(Matrix const& m) -> Matrix&{
        for(auto x=0;x<M;++x){
            for(auto y=0;y<N;++y){
                a[y][x]^=m[y][x];
            }
        }
        return *this;
    }
        
private:
    /**
     * @brief sorts the rows in non ascending order.
     * takes O(N*M) additional memory and O(N*M*log(N)) time complexity
     */
    static inline auto sort_matrix(Matrix& mat) -> void{
        using row_type=std::array<std::pair<std::bitset<M>, int>, N>;
        row_type bits;
        for(auto i=0;i<N;i++) {
            for(auto j=0;j<M;j++) bits[i].first[j]=mat[i][j];
            bits[i].second=i;
        }
        std::ranges::sort(bits,
        [](auto const& r1, auto const& r2){
            return r1.first.to_ullong()>r2.first.to_ullong();
        });
        
        auto res=Matrix();
        for(auto y=0;y<N;y++) res[y]=mat[bits[y].second];
        mat=res;
    }

    /**
     * @param mat matrix with sorted rows in non ascending order.
     */
    static inline auto row_echelon(Matrix& mat) -> void{
        for(auto y=0,x=0;y<N;y++){
            while(x!=M-1 && !mat[y][x]) x++;
            if(x==M-1) break;
            for(auto yt=y+1;yt<N && mat[yt][x];yt++){
                auto lcm=std::lcm(mat[y][x], mat[yt][x]);
                multiply_row(mat[y], -lcm/mat[y][x]);
                multiply_row(mat[yt], lcm/mat[yt][x]);
                add_rows(mat[yt], mat[y]);
            }   
        }
    }
    /**
     * @param mat matrix with sorted rows in non ascending order.
     */
    static inline auto reduced_row_echelon(Matrix& mat) -> void{
        // for(auto y=N-1,x=0;y>=0;y--){

        // }
    }

    static inline auto add_rows(std::array<T, M>& lhs, std::array<T, M> const& rhs) -> void{
        for(auto i=0;i<M;i++) lhs[i]+=rhs[i];
    }

    static inline auto multiply_row(std::array<T, M>& lhs, const T rhs) -> void{
        for(auto i=0;i<M;i++) lhs[i]*=rhs;
    }

    template<
        u_int32_t X
    >
    static inline auto multiply(Matrix const& lhs, Matrix<M, X, T> const& rhs, Matrix<N, X, T>& res) -> void{
        for(auto y=0;y<N;y++){
            for(auto x=0;x<X;x++){
                auto sum=T();
                for(auto i=0;i<M;i++) sum+=lhs[y][i]*rhs[i][x];
                res[y][x]=sum;
            }
        }
    }
    static inline auto multiply(Matrix& res, const T scalar) -> void{
        for(auto y=0;y<N;y++)
            for(auto x=0;x<M;x++)
                res[y][x]*=scalar;
    }
    static inline auto add(Matrix const& lhs, Matrix const& rhs, Matrix& res) -> void{
        for(auto y=0;y<N;y++)
            for(auto x=0;x<M;x++)
                res[y][x]=lhs[y][x]+rhs[y][x];
    }
    static inline auto subtract(Matrix const& lhs, Matrix const& rhs, Matrix& res) -> void{
        for(auto y=0;y<N;y++)
            for(auto x=0;x<M;x++)
                res[y][x]=lhs[y][x]-rhs[y][x];
    }
};

template<
  u_int32_t N,
  u_int32_t M,
  typename T
>
auto operator<<(std::ostream& os, Matrix<N, M, T> const& m) -> std::ostream&{
    os << "{\n";
    for(auto y=0;y<N;y++){
        os<<"\t{";
        for(auto x=0;x<M;x++){
            os<<m[y][x];
            if(x+1!=M) os<<", ";
        }
        os<<"},\n";
    }
    os<<'}';
    return os;
}
}
