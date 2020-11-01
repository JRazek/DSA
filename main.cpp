#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
using namespace std;
struct Leaf;
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
struct Connection{
    Leaf * l1;
    Leaf * l2;
    int toyType;
    int ID;
    Leaf * parent;
    Leaf * child;
    Connection(Leaf * l1, Leaf * l2, int streetID, int toyType){
        this->l1 = l1;
        this->l2 = l2;
        this->ID = streetID;
        this->toyType = toyType;
    }
    void setParent(Leaf * parent){//if true - the l1 else the l2
        if(l1 == parent || l2 == parent) {
            this->parent = parent;
            this->child = l1 == parent ? l2 : l1;
        }
        else
            cout<<"ERROR";
    }
};
struct Leaf{
    Connection * parentPath;
    vector<Connection *> connections;
    int id;
    int eulerTourID;
    int levelInTree;
    int firstOccurrenceInEuler = -1;
    int lastOccurrenceInEuler;

    void setParentPath(Connection * parentPath){
        this->parentPath = parentPath;
    }
};
bool belongsToSubTree(Leaf * root, Leaf * node){
    return (root->firstOccurrenceInEuler <= node->firstOccurrenceInEuler && root->lastOccurrenceInEuler >= node->firstOccurrenceInEuler);
}
void propagateParent(Leaf * root){
    stack<Leaf *> queue;
    queue.push(root);
    while (!queue.empty()){
        Leaf * subject = queue.top();
        queue.pop();
        for(int i = 0 ; i < subject->connections.size(); i ++){
            Connection * c = subject->connections.at(i);
            if(c != subject->parentPath) {
                Leaf * another = !(c->l1 == subject) ? c->l1 : c->l2;
                c->setParent(subject);
                another->setParentPath(c);
                queue.push(another);
            }
        }
    }
}

void eulerTourIndexing(Leaf * node, int * index, vector<Leaf *> &tourOrder, int level = 0){
    node->eulerTourID = *index;
    node->levelInTree = level;
    *index += 1;
    tourOrder.push_back(node);
    for(int i = 0; i < node->connections.size(); i ++){
        if(node->connections.at(i) != node->parentPath){
            eulerTourIndexing(node->connections.at(i)->child, index, tourOrder, ++level);
            tourOrder.push_back(node);
        }
    }
}
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
