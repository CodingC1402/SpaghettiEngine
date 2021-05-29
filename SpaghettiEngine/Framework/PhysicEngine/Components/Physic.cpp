#include "Physic.h"
#include "GameTimer.h"
#include "SMath.h"

void Physic::Update(const float& time)
{
	_accumulator += GameTimer::GetDeltaTime();
	if (_accumulator >= _step)
	{
		Step();
		_accumulator = SMath::modulo(_accumulator, _step);
	}
	return;
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

	for (auto& body : _body2D) {
		body->IntergateForces();
	}

	for (auto& contact : _contacts) {
		contact.Initialize();
	}

	for (auto& contact : _contacts) {
		contact.ApplyImpulse();
	}

	for (auto& body : _body2D) {
		body->IntergateVelocity();
	}

	for (auto& contact : _contacts) {
		contact.PositionalCorrection();
	}

	for (auto& body : _body2D) {
		body->SetForce(Vector3(0, 0, 0));
	}
	_contacts.clear();
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

void Physic::AddBody(Body2D* body)
{
	_body2D.push_back(body);
}

void Physic::RemoveBody(Body2D* body)
{
	for (auto it = _body2D.begin(); it != _body2D.end(); ++it)
		if (*it == body)
		{
			_body2D.erase(it);
			return;
		}
}
