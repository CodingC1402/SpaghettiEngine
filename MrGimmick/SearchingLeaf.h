#pragma once
#include "Node.h"

class SearchingLeaf : public LeafNode
{
public:
	State Tick() override;
	void CustomLoad(nlohmann::json& input) override;
	SNode Clone(WBTs tree) override;
private:
	float _scanRadius;

	static constexpr auto _radiusField = "Radius";
	NODE_REGISTER(SearchingLeaf);
};

