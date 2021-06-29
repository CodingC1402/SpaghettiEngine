#include "CircleCollider.h"
#include "Circle.h"
#include "Setting.h"
#include "Physic.h"
#include "DebugRenderer.h"

REGISTER_FINISH(CircleCollider);

CircleCollider::CircleCollider(PScene owner, bool isDisabled) : Collider2DBase(owner, isDisabled)
{
	_shapes.push_back(std::make_shared<Circle>());
}

void CircleCollider::OnFixedUpdate()
{
	if constexpr (Setting::IsDebugMode())
	{
		auto circle = std::dynamic_pointer_cast<Circle>(_shapes[0]);
		auto worldMatrix = GetWorldMatrix();
		worldMatrix._41 += circle->GetOffSetX();
		worldMatrix._42 += circle->GetOffSetY();

		DebugRenderer::DrawCircle(circle->GetRadius(), worldMatrix);
	}
}

void CircleCollider::Load(nlohmann::json& input)
{
	auto circle = std::dynamic_pointer_cast<Circle>(_shapes[0]);
	if (input[_radiusField] != nullptr)
		circle->SetRadius(input[_radiusField].get<float>());

	Collider2DBase::Load(input);
}
