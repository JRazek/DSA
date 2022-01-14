#ifndef UNION_FIND_H
#define UNION_FIND_H

template<typename T>
struct UnionNode{
	UnionNode* parent=this;
	T data=T();

	UnionNode* getRoot(){
		UnionNode* root=(parent==this ? this : &parent->getRoot());
		parent=root;
		return root;
	}
	static void joinUnion(UnionNode* n1, UnionNode* n2){
		UnionNode* r1=n1.getRoot();
		UnionNode* r2=n2.getRoot();
		r1=r2->parent;
	}
};

#endif //UNION_FIND_H