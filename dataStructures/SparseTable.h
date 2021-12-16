#ifndef SPARSE_TABLE_DSJR
#define SPARSE_TABLE_DSJR

#include <cmath>
#include <vector>

namespace stjr{
    //O(1) query time
    //O(nlogn) space
    //O(nlogn) preprocess

    template<
        typename T = u_int64_t,
        typename Y = void*,
        typename AssocFunc = std::greater<T>,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    class SparseTable{
    public:
        struct Node{
            T numericVal;
            Y data;
        };
        struct Range{
            u_int low;
            u_int high;
            Range common(Range rhs) const{
                return {std::max(low, rhs.low), std::min(high, rhs.high)};
            }
            bool operator==(const Range& r){
                return low==r.low&&high==r.high;
            };
        };

        std::vector<std::vector<Node>> table;
        AssocFunc assocFunc;

        template<
            typename AssocFuncT = AssocFunc,
            typename = typename std::enable_if<std::is_default_constructible<AssocFuncT>::value>::type 
        >
        SparseTable(const std::vector<Node>& data):
        SparseTable(data, AssocFunc()){}

        SparseTable(const std::vector<Node>& data, const AssocFunc& _assocFunc):
        table(std::log2(data.size()+1), std::vector<Node>(data.size())),
        assocFunc(_assocFunc){
            std::copy(data.begin(), data.end(), table.back().begin());
            for(u_int lvl=1; lvl<table.size(); lvl++){
                u_int s=std::pow(2, lvl);
                for(u_int l=0; l<=table[0].size()-s;l++){
                    Node left=rangeQuery({l, l+s/2-1});
                    Node right=rangeQuery({l+s/2, l+s-1});
                    table[lvl][l]=(assocFunc(left.numericVal, right.numericVal) ? left:right);
                }
            }
        }
        Node rangeQuery(Range r){
            u_int lvl=std::log2(r.high-r.low+1);
            u_int low=r.low;
            u_int high=r.high-std::pow(2, lvl)+1;
            return (assocFunc(table[lvl][low].numericVal, table[lvl][high].numericVal) ? table[lvl][low]:table[lvl][high]);
        }
    };
}
#endif //SPARSE_TABLE_DSJR