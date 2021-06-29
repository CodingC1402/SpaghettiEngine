#pragma once
#include "Collider2DBase.h"

class Circle;

class CircleCollider : public Collider2DBase
{
public:
	CircleCollider(PScene owner, bool isDisabled = false);

	void OnFixedUpdate() override;

	void Load(nlohmann::json& input) override;
protected:
	static constexpr auto _radiusField = "Radius";
private:
	REGISTER_START(CircleCollider);
};