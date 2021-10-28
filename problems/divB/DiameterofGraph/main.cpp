#include <bits/stdc++.h>

using namespace std;
int main(){
    int testCases;
    cin >>testCases;
    for(int testCase = 0; testCase < testCases; testCase++){
        int n, m, k;
        cin >> n >> m >> k;
        bool ok = true;

        long long maxPossibleEdges2 = n*(n-1)/2;
        if(k > 3 && n > 1){
            if(m > maxPossibleEdges2 || m < n-1)
                ok = false;
        }else if(k == 3 && m != maxPossibleEdges2){
            ok = false;
        }else if(k == 2 && m != 0){
            ok = false;
        }else if(k < 2){
            ok = false;
        }
        cout<<(ok ? "YES" : "NO")<<"\n";
    }
}