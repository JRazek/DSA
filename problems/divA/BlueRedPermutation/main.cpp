#include <bits/stdc++.h>
using namespace std;

int main(){
    int testCases;
    cin >> testCases;
    for (size_t testCase = 0; testCase < testCases; testCase++){
        int n;
        cin >> n;
        vector<int> vals(n);
        vector<bool> colors(n);
        for(int i = 0; i < n; i ++){
            cin >> vals[i];
        }
        string s;
        cin >> s;
        for(int i = 0; i < n; i ++){
            colors[i] = s[i] == 'B' ? 0 : 1;
        }
        for(int i = 0; i < vals.size(); i ++){
            if(vals[i])
        }
    }
    
    return 0;
}