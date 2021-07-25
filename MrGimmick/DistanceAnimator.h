#pragma once
#include "Render2DScriptBase.h"
#include "Sprite.h"

class DistanceAnimator : public Render2DScriptBase
{
public:
	void Draw(PCamera cam) override;
	void Load(nlohmann::json& input) override;

	void OnStart() override;
	void OnFixedUpdate() override;
	ScriptBase* Clone() const override;
private:
	float _distancePerFrame = 0;
	Vector3 _orginalPos;
	std::vector<SSprite> _sprites;
	SSprite _currentSprite;

	REGISTER_START(DistanceAnimator);
};

