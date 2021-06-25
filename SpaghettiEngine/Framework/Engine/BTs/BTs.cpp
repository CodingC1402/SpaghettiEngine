#include "BTs.h"
#include "Macros.h"
#include <fstream>

BTs::BTs()
{
	_root = NodeFactory::Create("Root");
}

SBTs BTs::CreateBehaviorTree()
{
	SBTs newTree = std::make_shared<BTs>();
	newTree->AssignPtr(newTree);
	return newTree;
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
	auto newTree = std::make_shared<BTs>();
	newTree->_bb = _bb->Clone();
	newTree->_root = _root->Clone(newTree);
	return newTree;
}

Node::State Root::Tick()
{
	return _child->Tick();
}