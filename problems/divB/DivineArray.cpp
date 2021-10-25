#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> perfQuery(vector<int>& arr){
    vector<vector<int>> result;
    vector<int> tmp;
    int i = 0;
    while(tmp != arr){
        tmp = arr;
        unordered_map<int, int> occ;
        result.push_back(arr);
        for(auto n : arr){
            if(occ.find(n) == occ.end())
                occ[n] = 0;
            occ[n] ++;
        }
        for(auto &n : arr){
            n = occ[n];
        }
        i++;
    }

    return result;
}

int main(){
    int testsCount;
    cin >> testsCount;
    for(int testNum = 0; testNum < testsCount; testNum ++){
        int arrSize;
        cin >> arrSize;
        vector<int> arr(arrSize);
        for(int i = 0; i < arrSize; i ++){
            int num;
            cin >> num;
            arr[i] = num;
        }
        vector<vector<int>> states = perfQuery(arr);
        int queriesCount;
        cin>>queriesCount;
        
        unordered_map<int, int> occurrencesCount;
        for(int queryNum = 0; queryNum < queriesCount; queryNum++){
            int x, k;
            cin >> x >> k;
            x--;
            k = min((int)states.size() - 1, k);
            std::cout<<states[k][x]<<"\n";
        }
    }
    return 0;
}
