#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
struct SparseTableMin{
    vector<int> values;
    vector<vector<int>*> sparseTable;
    SparseTableMin(vector<int> values){
        this->values = values;
        int columns = values.size();
        int rows = log2(values.size());
        for(int i = 0; i < rows; i ++){
            sparseTable.push_back(new vector<int>());
            int segmentSize = pow(2, i);
            for(int j = 0; j < columns - segmentSize + 1; j ++){
                if(i == 0){
                    sparseTable[0]->push_back(values[j]);
                } else{
                    sparseTable[i]->push_back(getMinimum(j, j + segmentSize - 1));
                }
            }
        }
    }
    int getMinimum(int from, int to){
        int length = to - from + 1;
        int rowNum = (int)(log2(length)) - 1;
        int fixSize = pow(2, rowNum);
        vector<int> * row = sparseTable[rowNum];
        int firstMin = row->at(from);
        int secondMin = row->at(to - fixSize + 1);
        int min = firstMin > secondMin ? secondMin : firstMin;
        return min;
    }
};
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
