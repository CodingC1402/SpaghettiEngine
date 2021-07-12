#include "Physic.h"
#include "GameTimer.h"
#include "RigidBody2D.h"
#include "SMath.h"
#include "Circle.h"
#include "GameObj.h"
#include "Game.h"
#include "ContainerUtil.h"
#include "Setting.h"
#include "DebugRenderer.h"

bool Physic::Update()
{
	bool isRunUpdate = false;
	_accumulator += GameTimer::GetDeltaTime();
	while (_accumulator >= _step)
	{
		// Update the parameter of all the objects in physic simulation
		for (auto& obj : _gameObjs) {
			obj->OnPhysicUpdate();
		}

		// The actual cal calculation of the physic engine
		Step();

		// Project the update back to the game engine obj
		for (auto& script : _rigid2DScripts) {
			script->AfterPhysicUpdate();
		}


		_accumulator -= _step;

		float interpolateNumber = _accumulator * (1 - _stepInterpolation);
		interpolateNumber = interpolateNumber > _baseStep ? _baseStep : interpolateNumber;
		_step = _baseStep * _stepInterpolation + interpolateNumber;

		// Clear the line renderer for debugging.
		if constexpr (Setting::IsDebugMode())
		{
			if (_accumulator < _step)
				DebugRenderer::Clear();
		}
		Game::FixUpdate();

		isRunUpdate = true;
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

	// This shit is scary AF stay away from this future me!!!.
	// Used for exit and enter event on collision : ^) wasteful af and useless af cause
	// of how unreliable they are.
	//for (auto iterator = _collidedBody.begin(); iterator != _collidedBody.end();)
	//{
	//	if ((*iterator)->SendExitEnterEvent())
	//	{
	//		auto toEraseIt = iterator;
	//		++iterator;
	//
	//		_collidedBody.erase(toEraseIt);
	//	}
	//	else
	//		++iterator;
	//}
	_contacts.clear();
}

void Physic::SetStep(const float& step)
{
	_step = step;
	_baseStep = step;
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
