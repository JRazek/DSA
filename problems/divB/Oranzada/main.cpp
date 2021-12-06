#include <bits/stdc++.h>
using namespace std;

auto main() -> int{
    int bC, minC;
    cin >> bC >> minC;
    list<int> bottles;
    for(int i = 0; i < bC; i ++){
        int s;
        cin >> s;
        bottles.push_back(s);
    }
    unordered_set<int> presentSet;
    int score=0;
    int time=0;
    auto high = bottles.begin();
    int dist = 0;
    for(auto low = bottles.begin(); low != bottles.end(), score != minC; ++low){
        if(dist<0){
            high = low;
            dist=0;
        }
        if(presentSet.find(*low) != presentSet.end()){
            while(high!=bottles.end() && presentSet.find(*high)!=presentSet.end()){
                ++high;
                dist++;
            }
            if(high == bottles.end()){
                break;
            }
            auto prev = high;
            --prev;
            int buffH = *high;
            bottles.erase(high);
            bottles.insert(low, buffH);
            time += dist;
            presentSet.insert(buffH); 
            high = prev;
        }else{
            presentSet.insert(*low);
        }
        score ++;
        dist--;
    }
    cout<<(score == minC ? time : -1);
}