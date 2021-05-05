#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"

class Collider2DScriptBase : public ScriptBase
{
public:
	Collider2DScriptBase(PScene owner);
	virtual void OnEnabled() override;
	virtual void OnDisabled() override;
	virtual void OnStart() override;

	virtual void SetTrigger(bool boolean);
	virtual void SetOffSet(Vector3 offset);
protected:
	Vector3 offSet;
	bool isTrigger = false;
	PRigidBody body = nullptr;
private:
	REGISTER_START(Collider2DScriptBase);
};