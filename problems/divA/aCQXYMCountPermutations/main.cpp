#include <bits/stdc++.h>
using namespace std;
#define MOD 1000000007;
int main(){
    int testCases;
    cin>>testCases;
    for(int testNum = 0; testNum < testCases; testNum++){
        int num;
        cin >> num;
        num *= 2;
        long long res = 1;
        for(int i = 3; i <= num; i++){
            res*=i;
            res%=MOD;
        }
        cout<<res<<"\n";
    }
    return 0;
}