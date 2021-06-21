#include "CircleCollider.h"
#include "Circle.h"
#include "LineRenderBase.h"
#include "Setting.h"

REGISTER_FINISH(CircleCollider);

CircleCollider::CircleCollider(PScene owner) : Collider2DBase(owner)
{
	_shapes.push_back(std::make_shared<Circle>());
}

void CircleCollider::Load(nlohmann::json& input)
{
	auto circle = std::dynamic_pointer_cast<Circle>(_shapes[0]);
	if (input[_radiusField] != nullptr)
		circle->SetRadius(input[_radiusField].get<float>());

	if constexpr (Setting::IsDebugMode())
	{
		_lineRenderer.emplace_back(new LineRendererBase(_owner));
		_lineRenderer[0]->AssignOwner(_ownerObj);
		_lineRenderer[0]->SetCircle(circle->GetRadius());
	}

	Collider2DBase::Load(input);
}
