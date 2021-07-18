#pragma once
#include "Node.h"

class SearchingLeaf : public LeafNode
{
public:
	State Tick() override;
private:
	float _scanRadius;
};

