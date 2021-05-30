#pragma once
#include "PhysicScriptBase.h"
#include "Body2D.h"

class RigidBody2D : public PhysicScriptBase
{
public:
	RigidBody2D(PScene owner, bool isDisabled = false);

	void OnDisabled() override;
	void OnEnabled() override;

	void AfterPhysicUpdate();

	void Load(nlohmann::json& input) override;
protected:
	SBody2D _body;

	static constexpr auto _velocityField = "Velocity";
	static constexpr auto _massField = "Mass";
	static constexpr auto _gravityScale = "GravityScale";
private:
	REGISTER_START(RigidBody2D);
};