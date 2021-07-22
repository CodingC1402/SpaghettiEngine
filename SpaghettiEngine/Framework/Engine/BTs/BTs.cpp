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

void BTs::ApplyChange(nlohmann::json& jsonFile, nlohmann::json& change)
{
	if (change[BTField::indexField].empty())
		change[BTField::indexField] = 0;
	
	if (unsigned level = change[BTField::indexField].get<unsigned>(); level == change[BTField::nodesField].size())
	{
		jsonFile[BTField::inputField][change[BTField::fieldField].get<std::string>()] = change[BTField::valueField];
	}
	else
	{
		change[BTField::indexField] = ++level;
		ApplyChange(jsonFile[BTField::childrenField][change[BTField::nodesField][level - 1].get<unsigned>()], change);
	}
}

void BTs::Load(const std::string& path, nlohmann::json& changes)
{
	std::fstream file;
	file.open(path);
	nlohmann::json input;
	file >> input;
	for (auto& change : changes)
	{
		ApplyChange(input, change);
	}

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
