#include <bits/stdc++.h>
using namespace std;

#define INF 3*1e6

using u_int = unsigned int;

u_int getJumps(u_int lvl, const vector<array<u_int, 2>> &vals, vector<array<u_int, 3>> &dp, bool start = false){
    if(lvl<=0){
        if(lvl == 0)
            dp[lvl][1] = 1;
        return 0;
    }
    if(lvl>=vals.size()){
        return INF;
    }
    
    if(!start){
        lvl += vals[lvl][1];
        if(dp[lvl][1] != 0)
            return INF;
    }
    dp[lvl][1] = -1;
    pair<int, int> best = {INF, -1};
    for(int i = 0; i <= vals[lvl][0]; i ++){
        int nextLvl = lvl - vals[lvl][1];
        int val = getJumps(nextLvl, vals, dp) + 1;
        if(val < best.first){
            best = {val, nextLvl};
        }
    }
    dp[lvl][0] = best.first;
    dp[lvl][2] = best.second;
    if(best.second != -1){
        dp[lvl][1] = 1;
    }else{
        dp[lvl][1] = -1;
    }
    return dp[lvl][0];
}

int main()
{
    u_int n;
    cin >> n;
    vector<array<u_int, 2>> values(n + 1);
    vector<array<u_int, 3>> dpArr(n+1);
    dpArr[0][0] = 0;
    dpArr[0][1] = 1;
    for(u_int i = 1; i < n; i ++){
        dpArr[i][1] = 0;
    }
    for (u_int i = 0; i < n*2; i++){
        u_int num;
        cin >> num;
        values[i%n + 1][i/n] = num;
    }
    int v = getJumps(n, values, dpArr, true);
    cout<<(dpArr[n][1] == 0 ? v : -1)<<"\n";
    return 0;
}