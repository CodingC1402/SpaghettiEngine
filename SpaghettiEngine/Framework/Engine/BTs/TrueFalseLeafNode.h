#pragma once
#include "Node.h"

class TrueLeaf : public LeafNode
{
public:
	State Tick() override;
	void CustomLoad(nlohmann::json&) override;
private:
	NODE_REGISTER(TrueLeaf);
};

class FalseLeaf : public LeafNode
{
	State Tick() override;
	void CustomLoad(nlohmann::json&) override;
private:
	NODE_REGISTER(FalseLeaf);
};