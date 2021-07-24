#pragma once
#include "Render2DScriptBase.h"
#include "AttackScript.h"
#include "Animation.h"

class StarCreation : public Render2DScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	void Draw(PCamera script) override;

	void Load(nlohmann::json& input) override;
	PScriptBase Clone() const override;
protected:
	AttackScript* _starScript = nullptr;
	SAnimation _starAnim;
	SSprite _currentSprite;

	Matrix4 _rotationMatrix;
	unsigned _numberOfStar	= 0;

	float _spinAngle		= 0.0f; // In 1 second;
	float _currentSpinAngle = 0.0f;

	float _baseRadius		= 0.0f;
	float _radius			= 0.0f;

	float _createTime		= 0.0f;
	float _counter			= 0.0f;

	float _animCounter		= 0.0f;
	unsigned _frame			= 0;
	bool _animEnded			= false;
private:
	REGISTER_START(StarCreation);
};

