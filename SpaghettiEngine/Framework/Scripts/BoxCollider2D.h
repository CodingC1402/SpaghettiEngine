#pragma once
#include "Collider2DScriptBase.h"

class BoxCollider2D : public Collider2DScriptBase
{
public:
	BoxCollider2D(PScene owner);

	virtual Vector3 GetSize();
	virtual void SetSize(Vector3 s);

	virtual void Load(nlohmann::json& inputObject) override;
protected:
	Vector3 size;
private:
	REGISTER_START(BoxCollider2D);
};