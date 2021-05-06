#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"

class RigidBody2D;

class Collider2DScriptBase : public ScriptBase
{
	friend class Physic;
public:
	Collider2DScriptBase(PScene owner);
	virtual void OnEnabled() override;
	virtual void OnDisabled() override;
	virtual void OnStart() override;

	virtual Vector3 GetPosition();
	
	virtual void SetTrigger(bool boolean);
	virtual void SetOffSet(Vector3 offset);

	virtual void IsCollided();
protected:
	Vector3 offSet;
	bool isTrigger = false;
	RigidBody2D* body = nullptr;
private:
	REGISTER_START(Collider2DScriptBase);
};