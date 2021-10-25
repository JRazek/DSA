#include <bits/stdc++.h>
using namespace std;

int main(){
    int stringsCount;
    cin>>stringsCount;
    for(int stringNum = 0; stringNum < stringsCount; stringNum++){
        string str; 
        cin >> str;
        int min = 'z';
        string a="", b="";
        bool done = false;
        for(int i = 0; i < str.size(); i++){
            if(str[i] < min)
                min = str[i];
        }
        for(int i = 0; i < str.size(); i++){
            if(str[i] == min && !done){
                a+=str[i];
                done = true;
            }else{
                b+=str[i];
            }
        }
        cout<<a<<" "<<b<<"\n";
    }
    return 0;
}