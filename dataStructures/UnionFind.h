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
	static void joinUnion(UnionNode* n1, UnionNode* n2){
		UnionNode* r1=n1->getRoot();
		UnionNode* r2=n2->getRoot();
		r1->parent=r2;
	}
};


#endif //UNION_FIND_H