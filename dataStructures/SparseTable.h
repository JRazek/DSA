#ifndef SPARSE_TABLE_DSJR
#define SPARSE_TABLE_DSJR
namespace dsjr{
    template<
        typename T = u_int64_t,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SparseTable{
        
    }
}
#endif //SPARSE_TABLE_DSJR