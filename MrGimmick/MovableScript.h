#pragma once
#include "ScriptBase.h"
#include <vector>
#include "SMath.h"

class MovableScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input) override;

	void OnFixedUpdate() override;
	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;

	PScriptBase Clone() const override;
private:
	float _speed = 0;

	int _factor = 1; // if return < 0 if go > 0

	int _currentIndex = 0;

	bool _isLoop = true;

	Vector3 _direction;
	Vector3 _des;
	std::vector<Vector3> _deltas;

	REGISTER_START(MovableScript);
 };