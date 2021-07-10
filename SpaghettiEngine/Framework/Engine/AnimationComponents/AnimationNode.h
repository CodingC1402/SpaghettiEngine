#pragma once

#include "Node.h"
#include "Animation.h"

class AnimationNode : public LeafNode
{
public:
	State Tick() override;
	
	void OnInterrupted() override;
	void CustomLoad(nlohmann::json& input) override;
	void Reset();

	SNode Clone(WBTs tree) override;
protected:
	SAnimation _anim;
	float _time = 0.0f;
	unsigned _index = 0u;
private:
	NODE_REGISTER(AnimationNode);
};