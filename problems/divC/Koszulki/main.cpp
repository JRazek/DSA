#include <bits/stdc++.h>

using namespace std;

auto main() -> int {
    int usersC, shirtsC;
    cin >> usersC >> shirtsC;
    vector<int> scores(usersC);
    for(int i = 0; i < usersC; i ++){
        cin >> scores[i];
    }
    sort(scores.begin(), scores.end(), [](int a, int b){
        return a > b;
    });
    int index = shirtsC - 1;
    while(index != scores.size() && scores[index - 1] != scores[index]){
        index++;
        shirtsC ++;
    }
    cout<<shirtsC;
}