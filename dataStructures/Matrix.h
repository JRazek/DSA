#ifndef JR_MATRIX_H
#define JR_MATRIX_H

#include <iostream>
#include <array>
#include <bitset>
#include <numeric>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cmath>

namespace jr{
template<
  std::size_t N,
  std::size_t M,
  typename T
>
struct Matrix{
    std::array<std::array<T, M>, N> a;
    
    Matrix():
    a({}){};
    
    Matrix(std::array<std::array<T, M>, N> const& arr):
    a(arr){}

    template<
        std::size_t X
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
    auto operator[](const std::size_t y) const -> std::array<T, M> const&{
        return a[y];
    }
    auto operator[](const std::size_t y) -> std::array<T, M>&{
        return a[y];
    }
    
    /**
     * @brief takes O(N^2*M) time and O(N^2*M) memory
     * 
     */
    auto determinant() const requires(N==M && N>3){

        /**
         * @brief copies matrix with erased row and column
         * 
         */
        auto make_matrix=[](const auto y_erased, const auto x_erased, Matrix const& m)->Matrix<N-1, M-1, T>{
            Matrix<N-1, M-1, T> res;
            for(auto y_dest=0, y_from=0; y_dest<N-1; y_dest++, y_from++){
                if(y_from==y_erased) y_from++;
                for(auto x_dest=0, x_from=0; x_dest<N-1 && y_from<N; x_dest++, x_from++){
                    if(x_from==x_erased) x_from++;
                    res[y_dest][x_dest]=m[y_from][x_from];
                }
            }
            return res;
        };

        auto det=T();
        const auto x=std::size_t(0); //fixed column for row reducing. Why not
        for(auto y=0;y<N;y++){
            det+=a[y][x]*std::pow(-1, y+x)*make_matrix(y, x, *this).determinant();
        }
        return det;
    }

    /**
     * @brief takes O(3*3) time and O(1) memory
     * 
     */
    auto determinant() const requires(N==M && N==3){
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

    /**
     * @brief takes O(2*2) time and O(1) memory
     * 
     */
    auto determinant() const requires(N==M && N==2){
        return a[0][0]*a[1][1]-a[0][1]*a[1][0];
    }
    auto determinant() const requires(N==M && N==1){
        return a[0][0];
    }
    auto gauss_elimination(){
        auto& mat=*this;
        
        /**
         * @brief sorts the rows in non ascending order.
         * takes O(N*M) additional memory and O(N*M*log(N)) time complexity
         */
        auto sort_rows =[](Matrix& mat) -> void{
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
        };

        sort_rows(mat);
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
        
    }

    static inline auto add_rows(std::array<T, M>& lhs, std::array<T, M> const& rhs) -> void{
        for(auto i=0;i<M;i++) lhs[i]+=rhs[i];
    }

    static inline auto multiply_row(std::array<T, M>& lhs, const T rhs) -> void{
        for(auto i=0;i<M;i++) lhs[i]*=rhs;
    }

    template<
        std::size_t X
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
  std::size_t N,
  std::size_t M,
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


#endif //JR_MATRIX_H
