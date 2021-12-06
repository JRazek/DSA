#include <bits/stdc++.h>

using namespace std;

int main(){
    int t;
    cin >> t;
    for(int tc = 0; tc < t; tc++){
        int n, k;
        cin >> n >> k;
        n++;
        long long sum = 1;
        int hourSum = 0;
        while(sum < n){
            sum += min(sum, (long long)k);
            hourSum ++;
        }
        cout<<hourSum<<'\n';
    }
    return 0;
}