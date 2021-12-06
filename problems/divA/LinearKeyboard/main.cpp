#include <bits/stdc++.h>
using namespace std;

int main(){
    int testCases;
    cin >> testCases;
    for (size_t testCase = 0; testCase < testCases; testCase++){
        long long x, n;
        cin >> x >> n;
        long long move;
        if(x % 2 == 0){
            long long even = n/2 + n%2;
            long long odd = n/2;
            int a1odd = 2, a1even = -1;
            move = (2*a1even - (even - 1)*2)/2*even + (2*a1odd + (even - 1)*2)/2*odd;
        }else{
            long long even = n/2;
            long long odd = n/2 + n%2;
            int a1odd = 1, a1even = -2;
            move = (2*a1even - (even - 1)*2)/2*even + (2*a1odd + (even - 1)*2)/2*odd;
        }
        cout<< x + move << "\n";
    }
    
    return 0;
}