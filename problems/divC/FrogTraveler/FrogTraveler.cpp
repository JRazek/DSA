#include <bits/stdc++.h>
using namespace std;

#define long long ll

#define INF 3*1e7

int getJumps(int lvl, vector<int> &dp, vector<int> &jumps, vector<int> &slips, vector<int> &path, set<int> &notRested){
    if(notRested.find(lvl) == notRested.end())
        return dp[lvl];
    notRested.erase(lvl);
    if(lvl == 0){
        dp[lvl] = 0;
        return dp[lvl];
    }
    pair<int, int> best = {INF, -1};

    //start from the node that is satisfied the first condition and end on the last that satisfied the last
    for(auto nr : notRested){
        if(lvl > nr && nr >= lvl - jumps[lvl]){
            int val = getJumps(nr + slips[nr], dp, jumps, slips, path, notRested) + 1;
            if(val < best.first){
                best = {val, nr};
            }
        }
    }
    dp[lvl] = best.first;
    path[lvl] = best.second;
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
    vector<int> dp(n+1, INF);
    vector<int> jumps(n+1, 0);
    vector<int> slips(n+1, 0);
    vector<int> path(n+1, 0);
    set<int> notRested;
    for(int i = 0; i <= n; i ++){
        notRested.insert(i);
    }
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
    getJumps(n, dp, jumps, slips, path, notRested);
    bool ok = dp[n] < INF;
    cout<<(ok ? dp[n] : -1)<<"\n";
    if(ok){
        list<int> str;
        getPath(n, path, slips, str);
        for(auto s : str){
            cout<<s<<" ";
        }
    }
    return 0;
}