#include <iostream>
#include <array>

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
    auto determinant() const requires(M==N){
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
        
private:

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
