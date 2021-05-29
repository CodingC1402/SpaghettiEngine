#pragma once
#include "Collider2DBase.h"

class Circle;

class CircleCollider : public Collider2DBase
{
public:
	CircleCollider(PScene owner);

	void Load(nlohmann::json& input) override;
protected:
	static constexpr auto _radiusField = "Radius";
	std::shared_ptr<Circle> _shapeCircle;
private:
	REGISTER_START(CircleCollider);
};