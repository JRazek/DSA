#include <bits/stdc++.h>
using namespace std;

#define long long ll

#define INF 3*1e7

int getJumps(int lvl, vector<int> &dp, vector<int> &dpStates, vector<int> &jumps, vector<int> &slips, vector<int> &path){
    auto lvlOk = [](int lvl, int n){
        return (lvl >= 0 && lvl <= n);
    };
    if(!lvlOk(lvl, dp.size()-1))
        return INF;
    if(lvl == 0){
        dpStates[0] = 1;
        dp[0] = 0;
    }
    if(dpStates[lvl] == 1)
        return dp[lvl];
    else if(dpStates[lvl] == -1)
        return INF;

    dpStates[lvl] = -1;
    pair<int, int> best = {INF, -1};
    for(int i = 0; i <= jumps[lvl]; i ++){
        int nextLvl = lvl - i;
        if(lvl == 9 && nextLvl == 4)
            cout<<"";
        if(lvl == 2 && nextLvl == 0)
            cout<<"";
        if(lvlOk(nextLvl, dp.size()-1)){
            int tmp = nextLvl;
            nextLvl += slips[nextLvl];
            int val = getJumps(nextLvl, dp, dpStates, jumps, slips, path) + 1;
            if(val < best.first){
                if(lvl == 4)
                    cout<<"";
                best = {val, nextLvl};
                path[lvl] = tmp;
            }
        }
    }
    if(best.second != -1){
        dpStates[lvl] = 1;
        dp[lvl] = best.first;
    }else{
        dpStates[lvl] = -1;
        dp[lvl] = INF;
    }
    
    return dp[lvl];
}
void getPath(int lvl, vector<int> &path, vector<int> &slips, list<int> &str){
    if(lvl == 0)
        return;
    int p = path[lvl];
    str.push_back(p);
    getPath(p + slips[p], path, slips, str);
}
int main(){
    int n;
    cin >> n;
    vector<int> dp(n+1, 0);
    vector<int> dpStates(n+1, 0);
    vector<int> jumps(n+1, 0);
    vector<int> slips(n+1, 0);
    vector<int> path(n+1, 0);
    for(int i = 0; i < n;  i++){
        int num;
        cin >> num;
        jumps[i+1] = num;
    }
    for(int i = 0; i < n;  i++){
        int num;
        cin >> num;
        slips[i+1] = num;
    }
    getJumps(n, dp, dpStates, jumps, slips, path);
    bool ok = dpStates[n] == 1;
    cout<<(ok ? dp[n] : -1)<<"\n";
    if(ok){
        list<int> str;
        getPath(n, path, slips, str);
        for(auto s : str){
            cout<<s<<" ";
        }
    }
    //frog traveler O(N^2 time).
    //to optimize!
    return 0;
}