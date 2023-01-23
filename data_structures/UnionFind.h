
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

	static std::size_t count(std::ranges::range auto& range){
		std::set<UnionNode*> unique;
		for(auto& i : range) unique.insert(&i.getRoot());
		return unique.size();
	}
};
