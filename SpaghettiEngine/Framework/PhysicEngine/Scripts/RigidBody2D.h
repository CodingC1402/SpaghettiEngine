#pragma once
#include "ScriptBase.h"
#include "Body2D.h"

class RigidBody2D : public ScriptBase
{
public:
	RigidBody2D(PScene owner, bool isDisabled = false);

	void OnDisabled() override;
	void OnEnabled() override;

	void Load(nlohmann::json& input) override;
protected:
	SBody2D _body;

	static constexpr auto _velocityField = "Velocity";
	static constexpr auto _massField = "Mass";
	static constexpr auto _gravityScale = "GravityScale";
private:
	REGISTER_START(RigidBody2D);
};