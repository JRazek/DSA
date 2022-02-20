struct UnionNode{
	UnionNode* parent;
	UnionNode(): parent(this){}
	UnionNode& getRoot(){
		return *(parent=(parent==this ? this : &parent->getRoot()));
	}
	void joinUnion(UnionNode& n1){
		auto& r1=n1.getRoot();
		r1.parent=&getRoot();
	}
};
