#pragma once
#include "ScriptBase.h"

// Only use this for enemies.
class RidablePlatformUpDown : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;

	void OnCollide(CollideEvent& e) override;
	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;

	ScriptBase* Clone() const override;
private:
	Vector3 _delta;
	int _factor = 1;

	float _delay = 0;
	float _delayCounter = 0;

	std::set<GameObj*> _objects;
	bool _collideInLoop = false;

	GameObj* _platform;

	REGISTER_START(RidablePlatformUpDown);
};

