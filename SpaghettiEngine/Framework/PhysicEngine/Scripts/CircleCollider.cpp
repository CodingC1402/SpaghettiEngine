#include "CircleCollider.h"
#include "Circle.h"
#include "LineRenderBase.h"
#include "Setting.h"

REGISTER_FINISH(CircleCollider);

CircleCollider::CircleCollider(PScene owner) : Collider2DBase(owner)
{
	_shapeCircle = std::make_shared<Circle>();
	_shapes.push_back(_shapeCircle);
	_name = TYPE_NAME(CircleCollider);
}

void CircleCollider::Load(nlohmann::json& input)
{
	if (input[_radiusField] != nullptr)
		_shapeCircle->SetRadius(input[_radiusField].get<float>());

	if constexpr (Setting::IsDebugMode())
	{
		_lineRenderer.emplace_back(new LineRendererBase(_owner));
		_lineRenderer[0]->AssignOwner(_ownerObj);
		_lineRenderer[0]->SetCircle(_shapeCircle->GetRadius());
	}

	Collider2DBase::Load(input);
}
