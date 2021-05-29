#pragma once

#include "ScriptBase.h"
#include "ObservePattern.h"
#include "Body2D.h"
#include "Shape.h"

class Collider2DBase : public Observer, public ScriptBase
{
public:
	Collider2DBase(PScene owner);

	void OnEnabled() override;
	void OnDisabled() override;
	void OnChange() override;

	void Load(nlohmann::json& input) override;
protected:
	void ChangeBody(WBody2D body);
protected:
	WBody2D _body;
	SShape _shape;

	static constexpr auto _offSetXField = "OffSetX";
	static constexpr auto _offSetYField = "OffSetY";
};