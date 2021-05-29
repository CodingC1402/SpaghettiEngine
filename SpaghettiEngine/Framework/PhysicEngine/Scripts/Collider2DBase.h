#pragma once

#include "ScriptBase.h"
#include "LineRenderBase.h"
#include "Body2D.h"
#include "Shape.h"

class Collider2DBase : public ScriptBase
{
public:
	Collider2DBase(PScene owner);

	void OnEnabled() override;
	void OnDisabled() override;
	void OnChange();

	void AssignOwner(const PGameObj& gameObj) override;

	void Load(nlohmann::json& input) override;
	~Collider2DBase();
protected:
	void SetLineRendererOwner();
	void ChangeBody(WBody2D body);
protected:
	WBody2D _body;
	SShape _shape;
	LineRendererBase* _lineRenderer;

	static constexpr auto _offSetXField = "OffSetX";
	static constexpr auto _offSetYField = "OffSetY";
};