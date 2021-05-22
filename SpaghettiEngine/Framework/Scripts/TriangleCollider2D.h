#pragma once
#include "Collider2DScriptBase.h"

class TriangleCollider2D : public Collider2DScriptBase
{
public:
	TriangleCollider2D(PScene owner);

	virtual void Set(Vector3 al, Vector3 bt, Vector3 gm);

	virtual Vector3 GetA();
	virtual Vector3 GetB();
	virtual Vector3 GetC();

	virtual void Create(Vector3 al, Vector3 bt, Vector3 gm);
	virtual void Load(nlohmann::json& inputObject) override;
protected:
	Vector3 A;
	Vector3 B;
	Vector3 C;
private:
	REGISTER_START(TriangleCollider2D);
};