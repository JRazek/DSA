namespace dsjr{
    template<
        typename T = u_int64_t,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SparseTable{
        
    }
}