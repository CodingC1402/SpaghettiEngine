#include "CircleCollider.h"
#include "Circle.h"
#include "LineRenderBase.h"
#include "Setting.h"

REGISTER_FINISH(CircleCollider);

CircleCollider::CircleCollider(PScene owner) : Collider2DBase(owner)
{
	_shapeCircle = std::make_shared<Circle>();
	_shape = _shapeCircle;
	_name = TYPE_NAME(CircleCollider);
}

void CircleCollider::Load(nlohmann::json& input)
{
	if (input[_radiusField] != nullptr)
		_shapeCircle->SetRadius(input[_radiusField].get<float>());
	Collider2DBase::Load(input);

	if constexpr (Setting::IsDebugMode())
	{
		_lineRenderer->SetCircle(_shapeCircle->GetRadius());
	}
}
