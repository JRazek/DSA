#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr ll MOD = 1e9+7;

auto main() -> int {
    int n, m;
    cin >> n >> m;
    auto modPow = [](ll a, int b) -> ll{
        a%=MOD;
        ll res = 1;
        for(int i = 0; i < b; i ++){
            res*=a;
            res%=MOD;
        }
        return res;
    };
    auto isValid = [](int l, int n, int m) -> bool{
        return l>1 && l<n-1 && m>1;
    };
    auto calcState = [&](int l, int n, int m) -> ll{
        return isValid(l, n, m) ? (m*modPow(m, l-2) * modPow(m-1, n-l-2))%MOD : 0;
    };
    ll sum = m*modPow(m, n-2)%MOD;
    for(int l = 2; l < n-1; l++){
        sum+=calcState(l, n, m);
        sum%=MOD;
    }
    cout<<(n>1?sum:0);
}