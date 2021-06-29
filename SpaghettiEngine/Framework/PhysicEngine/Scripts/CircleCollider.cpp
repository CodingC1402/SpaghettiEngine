#include "CircleCollider.h"
#include "Circle.h"
#include "Setting.h"
#include "Physic.h"

REGISTER_FINISH(CircleCollider);

CircleCollider::CircleCollider(PScene owner, bool isDisabled) : Collider2DBase(owner, isDisabled)
{
	_shapes.push_back(std::make_shared<Circle>());
}

void CircleCollider::Load(nlohmann::json& input)
{
	auto circle = std::dynamic_pointer_cast<Circle>(_shapes[0]);
	if (input[_radiusField] != nullptr)
		circle->SetRadius(input[_radiusField].get<float>());

	Collider2DBase::Load(input);
}
