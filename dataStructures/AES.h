#ifndef JR_AES_H
#define JR_AES_H


#include <array>
#include <algorithm>
#include "Matrix.h"


namespace jr{
namespace crypt{



enum class AES_TYPE{
    AES_256 = 256u
};
template <typename Enumeration>
constexpr auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

template<int N>
constexpr auto build_s_box(){
    constexpr auto seed=__TIMESTAMP__;
    std::default_random_engine e;

    Matrix<N, N, u_int8_t> s_box;//TODO - make it an ascending arithmetic sequence 0 to N*N-1;
    
    // std::ranges::shuffle()//shuffle s_box to a random permutation
    return s_box;
}

template<
    AES_TYPE aes_type
>
class AES{
    constexpr static inline auto N=as_integer(aes_type);
    constexpr static inline auto s_box=build_s_box<N>();

};



}
};



#endif //JR_AES_H
