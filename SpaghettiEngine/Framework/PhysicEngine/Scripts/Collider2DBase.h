#pragma once
#include "PhysicScriptBase.h"
#include "Body2D.h"
#include "Shape.h"
#include "CollideEvent.h"

class LineRendererBase;
typedef std::shared_ptr<LineRendererBase> SLineRendererBase;

class Collider2DBase : public PhysicScriptBase
{
public:
	Collider2DBase(PScene owner, bool isDisabled = false);

	void OnEnabled() override;
	void OnDisabled() override;
	void OnChange();

	void SetGameObject(const PGameObj& gameObj) override;

	void Load(nlohmann::json& input) override;
	bool CallDestroy() override;

	PScriptBase Clone() const override;
	~Collider2DBase();
protected:
	void SetLineRendererOwner();
	void ChangeBody(WBody2D body);
protected:
	WBody2D _body;
	std::vector<SShape> _shapes;
	std::vector<LineRendererBase*> _lineRenderer;

	static constexpr auto _offSetXField = "OffSetX";
	static constexpr auto _offSetYField = "OffSetY";
};