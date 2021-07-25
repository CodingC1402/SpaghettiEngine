#pragma once
#include "ScriptBase.h"
#include "SMath.h"
#include <list>
#include <functional>
#include <vector>

class MovableScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input) override;

	void OnFixedUpdate() override;
	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;

	void AddArrivedEvent(const std::function<void(const Vector3& destination)>& fun);
	void ClearEvent();

	[[nodiscard]] float GetDelay() const noexcept;
	void Dispatch();
	// Ignore delay
	void ForceDispatch();

	PScriptBase Clone() const override;
private:
	void Arrived();
private:
	float _speed = 0.0f;
	int _factor = 1; // if return < 0 if go > 0

	int _currentIndex = 0;

	bool _isInDelay = false;
	float _delay = 0;
	float _counter = 0;

	// Meaning this object will automatically move to next point.
	bool _isLoop = true;
	// If loop is false then you need to manually call dispatch.
	bool _callDispatch = false;

	Vector3 _direction;
	Vector3 _des;
	std::vector<Vector3> _deltas;

	bool _needToCallEvent = true;
	std::list<std::function<void(const Vector3& destination)>> _delegate;

	REGISTER_START(MovableScript);
 };