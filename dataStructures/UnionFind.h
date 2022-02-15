#ifndef UNION_FIND_H
#define UNION_FIND_H

template<typename T>
struct UnionNode{
	UnionNode* parent;
	T data;
	UnionNode(): parent(this), data(T()){}
	UnionNode* getRoot(){
		parent=(parent==this ? this : parent->getRoot());
		return parent;
	}
	void joinUnion(UnionNode* n1){
		UnionNode* r1=n1->getRoot();
		r1->parent=getRoot();
	}
};


#endif //UNION_FIND_H