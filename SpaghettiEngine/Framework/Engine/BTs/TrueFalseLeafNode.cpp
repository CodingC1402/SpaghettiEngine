#include "TrueFalseLeafNode.h"

Node::State TrueLeaf::Tick()
{
	return State::Success;
}

void TrueLeaf::CustomLoad(nlohmann::json&)
{
	return;
}

Node::State FalseLeaf::Tick()
{
	return State::Failure;
}

void FalseLeaf::CustomLoad(nlohmann::json&)
{
	return;
}
