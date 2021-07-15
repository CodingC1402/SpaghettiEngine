#include "BTs.h"
#include "Macros.h"

#include "AnimationTree.h"

#include <fstream>

void BEHAVIOR_TREE_REGISTER()
{
	BTs();
	AnimationTree();
}

BehaviorTreeFactory::TreeRegister<BTs> BTs::REGISTRY_KEY_BEHAVIOR_TREE("BTs");

BTs::BTs()
{
	_root = NodeFactory::Create("Root");
}

std::string BTs::GetType()
{
	return "BTs";
}

void BTs::Load(const std::string& path)
{
	std::fstream file;
	file.open(path);
	nlohmann::json input;
	file >> input;

	_bb = BlackBoard::Load(input[BTField::blackBoardField]);
	_root->Load(input, _this);
}

void BTs::AssignPtr(WBTs ptr)
{
	_this = ptr;
}

Node::State BTs::Tick()
{
	return _root->Tick();
}

WBlackBoard BTs::GetBlackBoard()
{
	return _bb;
}

SBTs BTs::Clone()
{
	auto newTree = BehaviorTreeFactory::MakeShared<BTs>(GetType());
	newTree->_bb = _bb->Clone();
	newTree->_root = _root->Clone(newTree);
	return newTree;
}

Node::State Root::Tick()
{
	return _child->Tick();
}
