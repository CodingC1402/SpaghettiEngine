#pragma once
#include "Collider2DScriptBase.h"

class BoxCollider2D : Collider2DScriptBase
{
public:
	BoxCollider2D(PScene owner);

	virtual Vector3 GetSize();
	virtual void SetSize(Vector3 s);
	
	virtual void Load(nlohmann::json& input) override;

	virtual void IsCollided(BoxCollider2D a);
protected:
	Vector3 size;
private:
	REGISTER_START(BoxCollider2D);
};