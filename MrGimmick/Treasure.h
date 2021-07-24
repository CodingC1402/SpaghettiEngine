#pragma once
#include "ScriptBase.h"
#include "Animation.h"

class Treasure : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void Load(nlohmann::json&) override;
private:
	unsigned _point;
	SSprite _sprite;

	SAnimation _smallToBig;
	unsigned _smallIndex;
	float _smallCounter = 0.0f;

	SAnimation _bigToSmall;
	unsigned _bigIndex;
	float _bigCounter;

	std::vector<Vector3> _smallCenters;
	std::vector<Vector3> _bigCenters;

	REGISTER_START(Treasure);
};

