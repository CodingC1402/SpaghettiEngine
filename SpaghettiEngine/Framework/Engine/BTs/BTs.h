#pragma once
#include "Node.h"
#include "BlackBoard.h"
#include <unordered_map>

class Root : public DecoratorNode
{
public:
	Node::State Tick() override;
private:
	NODE_REGISTER(Root);
};

class BTs;
typedef std::shared_ptr<BTs> SBTs;
typedef std::weak_ptr<BTs> WBTs;

class BehaviorTreeFactory
{
public:
	template <typename Tree>
	class TreeRegister
	{
	public:
		TreeRegister(const std::string& type);
	};
public:
	template<typename T>
	static std::shared_ptr<T> MakeShared(const std::string& type);
protected:
	template<typename T>
	static void Register(const std::string& type);
private:
	template<typename T>
	static SBTs MakeShared(); 
private:
	inline static std::unordered_map<std::string, std::shared_ptr<BTs>(*)()> _factory;
};

#define REGISTER_BEHAVIOR_TREE_START(Type) public: std::string GetType() override; private: static BehaviorTreeFactory::TreeRegister<Type> REGISTRY_KEY_BEHAVIOR_TREE
#define REGISTER_BEHAVIOR_TREE_FINISH(Type) std::string Type::GetType() { return #Type; } BehaviorTreeFactory::TreeRegister<Type> Type::REGISTRY_KEY_BEHAVIOR_TREE(#Type);
#define MAKE_SHARE_BT(Type) BehaviorTreeFactory::MakeShared<Type>(#Type)

class BTs
{
public:
	virtual std::string GetType();

	void Load(const std::string& path);
	void AssignPtr(WBTs ptr);
	BTs();
	Node::State Tick();
	[[nodiscard]] WBlackBoard GetBlackBoard();
	[[nodiscard]] virtual SBTs Clone();
protected:
	SBlackBoard _bb;
	WBTs _this;
	SNode _root;
private:
	static BehaviorTreeFactory::TreeRegister<BTs> REGISTRY_KEY_BEHAVIOR_TREE;
};

template<typename T>
inline SBTs BehaviorTreeFactory::MakeShared()
{
	auto newTree = std::make_shared<T>();
	newTree->AssignPtr(newTree);
	return newTree;
}

template<typename T>
inline void BehaviorTreeFactory::Register(const std::string& type)
{
	_factory.insert(std::pair<std::string, std::shared_ptr<BTs>(*)()>(type, &BehaviorTreeFactory::MakeShared<T>));
}

template<typename Tree>
inline BehaviorTreeFactory::TreeRegister<Tree>::TreeRegister(const std::string& type)
{
	BehaviorTreeFactory::Register<Tree>(type);
}

template<typename T>
std::shared_ptr<T> BehaviorTreeFactory::MakeShared(const std::string& type)
{
	return std::dynamic_pointer_cast<T>((*_factory[type])());
}