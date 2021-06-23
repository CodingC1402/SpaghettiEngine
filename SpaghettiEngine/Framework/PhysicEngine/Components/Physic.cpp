#include "Physic.h"
#include "GameTimer.h"
#include "RigidBody2D.h"
#include "SMath.h"
#include "Circle.h"
#include "GameObj.h"
#include "ContainerUtil.h"

bool Physic::Update()
{
	bool isRunUpdate = false;
	_accumulator += GameTimer::GetDeltaTime();
	if (_accumulator >= _step)
	{
		// Update the parameter of all the objects in physic simulation
		for (auto& obj : _gameObjs) {
			obj->OnPhysicUpdate();
		}

		// The actual cal calculation of the physic engine
		Step();

		_accumulator = SMath::modulo(_accumulator, _step);
		isRunUpdate = true;

		// Project the update back to the game engine obj
		for (auto& script : _rigid2DScripts) {
			script->AfterPhysicUpdate();
		}
	}
	return isRunUpdate;
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

	for (auto iterator = _collidedBody.begin(); iterator != _collidedBody.end();)
	{
		if ((*iterator)->SendExitEnterEvent())
		{
			auto toEraseIt = iterator;
			++iterator;

			_collidedBody.erase(toEraseIt);
		}
		else
			++iterator;
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

void Physic::AddRigid2DScript(RigidBody2D* script)
{
	ContainerUtil::EmplaceBackUnique(_rigid2DScripts, script);
}

void Physic::RemoveRigid2DScript(RigidBody2D* script)
{
	ContainerUtil::Erase(_rigid2DScripts, script);
}

void Physic::AddShape(Shape* shape)
{
	ContainerUtil::EmplaceBackUnique(_shapes, shape);
}

void Physic::RemoveShape(Shape* shape)
{
	ContainerUtil::Erase(_shapes, shape);
}

void Physic::AddBody(Body2D* body)
{
	ContainerUtil::EmplaceBackUnique(_body2D, body);
}

void Physic::RemoveBody(Body2D* body)
{
	ContainerUtil::Erase(_body2D, body);
	RemoveCollidedBody(body);
}

void Physic::AddCollidedBody(Body2D* body)
{
	_collidedBody.emplace(body);
}

void Physic::RemoveCollidedBody(Body2D* body)
{
	auto it = _collidedBody.find(body);
	if (it != _collidedBody.end())
		_collidedBody.erase(it);
}

void Physic::AddGameObj(GameObj* gameObj)
{
	ContainerUtil::EmplaceBackUnique(_gameObjs, gameObj);
}

void Physic::RemoveGameObj(GameObj* gameObj)
{
	ContainerUtil::Erase(_gameObjs, gameObj);
}
