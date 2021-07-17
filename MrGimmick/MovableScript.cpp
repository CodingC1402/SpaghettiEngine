#include "FieldNames.h"
#include "MovableScript.h"
#include "GameTimer.h"
#include "Physic.h"
#include "Debug.h"

REGISTER_FINISH(MovableScript, ScriptBase) {}

void MovableScript::Load(nlohmann::json& input)
{
	// load vận tốc.
	_speed = input[Fields::MovableObject::_speed] == nullptr ? 0 : input[Fields::MovableObject::_speed].get<float>();

	// load isLoop.
	_isLoop = input[Fields::MovableObject::_isLoop] == nullptr ? false : input[Fields::MovableObject::_isLoop].get<bool>();

	// load path.
	auto path = input[Fields::MovableObject::_path];
	for (auto point: path)
	{
		float x = point[0].get<float>();
		float y = point[1].get<float>();
		float z = point[2].get<float>();
		_deltas.push_back(Vector3(x, y, z));
	}
}

void MovableScript::OnEnabled()
{
	_des += GetGameObject()->GetTransform().GetWorldTransform();
}

void MovableScript::OnDisabled()
{
	_des -= GetGameObject()->GetTransform().GetWorldTransform();
}

void MovableScript::OnFixedUpdate() {
	if (_deltas.empty())
		return;

	auto delta = _des - GetWorldTransform();
	if (delta.Dot(_deltas[_currentIndex] * _factor) < 0 && _isLoop)
	{
		// positional correction
		GetGameObject()->GetTransform().Translate(delta);

		_currentIndex += _factor;
		if (_currentIndex < 0)
		{
			_factor = 1;
			_currentIndex = 0;
		}
		else if (_currentIndex == _deltas.size())
		{
			_factor = -1;
			_currentIndex = _deltas.size() - 1;
		}

		_des = _deltas[_currentIndex] * _factor + GetWorldTransform();
		_direction = _deltas[_currentIndex].GetUnitVector();
	}
	else
	{
		GetGameObject()->GetTransform().Translate(_direction * _speed * _factor * Physic::GetStep());
	}
}

void MovableScript::OnStart()
{
	_des = _deltas[_currentIndex];
	_direction = _des.GetUnitVector();
}

PScriptBase MovableScript::Clone() const
{
	auto clone = dynamic_cast<MovableScript*>(ScriptBase::Clone());

	clone->_speed = _speed;
	clone->_isLoop = _isLoop;
	clone->_deltas = std::vector<Vector3>(_deltas);

	return clone;
}