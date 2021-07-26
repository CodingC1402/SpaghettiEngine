#pragma once
#include "PhysicScriptBase.h"
#include "Body2D.h"
#include "Shape.h"
#include "CollideEvent.h"

class Collider2DBase : public PhysicScriptBase
{
public:
	Collider2DBase(PScene owner, bool isDisabled = false);

	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;
	void OnChange();

	void SetGameObject(const PGameObj& gameObj) override;

	void Load(nlohmann::json& input) override;
	bool CallDestroy() override;

	void SetIsTrigger(bool value);
	[[nodiscard]] bool IsTrigger() const;

	PScriptBase Clone() const override;
protected:
	void ChangeBody(WBody2D body);
	void SetOwnerForShapes();
protected:
	WBody2D _body;
	bool _isTrigger = false;
	std::vector<SShape> _shapes;
};