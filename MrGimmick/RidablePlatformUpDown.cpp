#include "RidablePlatformUpDown.h"
#include "GameTimer.h"
#include "FieldNames.h"

REGISTER_FINISH(RidablePlatformUpDown, ScriptBase) {}

void RidablePlatformUpDown::OnStart()
{
	_platform = GetGameObject()->GetChildContainer()[0];
}

void RidablePlatformUpDown::OnUpdate()
{
	_delayCounter += GameTimer::GetDeltaTime();
	while (_delayCounter > _delay)
	{
		_platform->GetTransform().Translate(_delta * static_cast<float>(_factor));
		for (auto& obj : _objects)
			obj->GetTransform().Translate(_delta * static_cast<float>(_factor));

		_factor *= -1;
		_delayCounter -= _delay;
	}
}

void RidablePlatformUpDown::OnCollide(CollideEvent& e)
{

	if (e.GetGameObject()->GetPhysicComponent().GetRigidBody2DScript())
	{
		_collideInLoop = true;
		_objects.clear();
		_objects.insert(e.GetGameObject());
	}
}

void RidablePlatformUpDown::OnFixedUpdate()
{
	if (!_collideInLoop)
		_objects.clear();
	_collideInLoop = false;
}

void RidablePlatformUpDown::Load(nlohmann::json& input)
{
	_delay = input[Fields::RidablePlatformUpDown::_delay].get<float>();
	_delta = input[Fields::RidablePlatformUpDown::_delta];
}

ScriptBase* RidablePlatformUpDown::Clone() const
{
	auto clone = dynamic_cast<RidablePlatformUpDown*>(ScriptBase::Clone());

	clone->_delay = _delay;
	clone->_delta = _delta;

	return clone;
}
