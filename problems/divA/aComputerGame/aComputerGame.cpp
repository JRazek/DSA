#include <bits/stdc++.h>
using namespace std;

bool dfs(pair<int,int> point, vector<vector<bool>> &grid, vector<vector<bool>> &visited){
    if(!(point.first < grid.size() && point.second >= 0 && point.second < 2 && !visited[point.first][point.second] && !grid[point.first][point.second]))
        return 0;
    if(point.first == grid.size() - 1 && point.second == 1)
        return true;
    bool ok = false;
    visited[point.first][point.second] = true;
    if(dfs({point.first + 1, point.second}, grid, visited))
        ok = true;
    else if(dfs({point.first, point.second + 1}, grid, visited))
        ok = true;
    else if(dfs({point.first, point.second - 1}, grid, visited))
        ok = true;
    else if(dfs({point.first + 1, point.second + 1}, grid, visited))
        ok = true;
    else if(dfs({point.first + 1, point.second - 1}, grid, visited))
        ok = true;
    return ok;
}

int main(){
    int testsCount;
    cin >> testsCount;
    for(int testNum = 0; testNum < testsCount; testNum ++){
        int columns;
        cin >> columns;
        vector<vector<bool>> grid(columns, vector<bool>(2, 0));
        vector<vector<bool>> visited = grid;

        string r1;
        cin >> r1;
        string r2;
        cin >> r2;
        for(int i = 0; i < columns; i++){
            grid[i][0] = r1[i] -'0';
        }
        for(int i = 0; i < columns; i++){
            grid[i][1] = r2[i] -'0';;
        }
        cout<< (dfs({0,0}, grid, visited) ? "YES" : "NO") << "\n";
    }
    return 0;
}