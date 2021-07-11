#include "Surface.h"
#include "ScriptField.h"
#include "Physic.h"
#include <functional>

REGISTER_FINISH(Surface, ScriptBase) {}

void Surface::OnCollide(CollideEvent& e)
{
	auto obj = e.GetGameObject();
	auto rb = obj->GetPhysicComponent().GetRigidBody2DScript();
	if (!rb || GetGameObject()->GetTag().Collide(obj->GetTag()))
		return;

	_collideNow.emplace(rb);
}

void Surface::OnFixedUpdate()
{
	_collided = std::move(_collideNow);
	_collideNow.clear();
	for (auto& rb : _collided)
	{
		rb->GetGameObject()->GetTransform().Translate(_moveVec * Physic::GetStep());
	}
}

NLOHMANN_JSON_SERIALIZE_ENUM(Surface::Direction, {
	{Surface::Direction::Up, "Up"},
	{Surface::Direction::Down, "Down"},
	{Surface::Direction::Left, "Left"},
	{Surface::Direction::Right, "Right"},
	})

void Surface::Load(nlohmann::json& input)
{
	ScriptBase::Load(input);

	_direction = input[Fields::Surface::GetDirectionField()].get<Direction>();
	auto vel = input[Fields::Surface::GetVelocityField()].get<float>();
	_ignoreTags = Tag(input[Fields::Surface::GetIgnoreTag()]);

	switch (_direction)
	{
	case Surface::Direction::Down:
		vel *= -1;
		[[fallthrough]];
	case Surface::Direction::Up:
		_moveVec.y = vel;
		break;
	[[likely]] case Surface::Direction::Left:
		vel *= -1;
		[[fallthrough]];
	[[likely]] case Surface::Direction::Right:
		_moveVec.x = vel;
		break;
	}
}

ScriptBase* Surface::Clone() const
{
	auto clone = dynamic_cast<Surface*>(ScriptBase::Clone());

	clone->_moveVec = _moveVec;
	clone->_direction = _direction;

	return clone;
}
