#pragma once
#include "ScriptBase.h"
#include "Animation.h"
#include "Render2DScriptBase.h"

class Treasure : public Render2DScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void OnUpdate() override;
	void Draw(PCamera cam) override;
	void Load(nlohmann::json& input) override;
	
	ScriptBase* Clone() const override;
private:
	unsigned _point =  0;
	SSprite _sprite;

	SAnimation _smallToBig;
	unsigned _smallIndex = 0;
	float _smallCounter = 0.0f;

	SAnimation _bigToSmall;
	unsigned _bigIndex = 0;
	float _bigCounter = 0.0f;
	bool _isUsed = false;

	std::vector<Vector3> _smallCenters;
	std::vector<Vector3> _bigCenters;

	REGISTER_START(Treasure);
};

