#include "Physic.h"
#include "GameTimer.h"

float Physic::Update(const float& time)
{
	_accumulator += GameTimer::GetDeltaTime();
	while (_accumulator >= _step)
	{
		Step();
		_accumulator -= _step;
	}
	return _accumulator / _step;
}

void Physic::Step()
{
	_contacts.clear();
	for (int i = 0; i < _shapes.size(); i++)
	{
		for (int j = i + 1; j < _shapes.size(); j++)
		{
			Collision newCollide(_shapes[i], _shapes[j]);
			if (newCollide.Solve())
				_contacts.emplace_back(newCollide);
		}
	}
}

void Physic::SetStep(const float& step)
{
	_step = step;
}

float Physic::GetStep()
{
	return _step;
}

void Physic::SetGravity(const float& gravity)
{
	_gravity.y = -gravity;
}

const Vector3& Physic::GetGravity()
{
	return _gravity;
}

void Physic::AddShape(Shape* shape)
{
	for (auto& addedShape : _shapes)
	{
		if (addedShape == shape)
			return;
	}
	_shapes.push_back(shape);
}

void Physic::RemoveShape(Shape* _shape)
{
	for (auto it = _shapes.begin(); it != _shapes.end(); ++it)
	{
		if (*it == _shape)
		{
			_shapes.erase(it);
			return;
		}
	}
}
