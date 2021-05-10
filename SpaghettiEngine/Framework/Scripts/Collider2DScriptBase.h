#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"

class RigidBody2D;
typedef class Collider2DScriptBase* PCollider2DScriptBase;

class Collider2DScriptBase : public ScriptBase
{
	friend class Physic;
public:
	Collider2DScriptBase(PScene owner);
	virtual void OnEnabled() override;
	virtual void OnDisabled() override;
	virtual void OnStart() override;

	virtual Vector3 GetPosition();
	virtual long GetID();
	
	virtual void SetTrigger(bool boolean);
	virtual void SetOffSet(Vector3 offset);

	virtual void Load(nlohmann::json& inputObject) override;
protected:
	Vector3 offSet;
	bool isTrigger = true;
	RigidBody2D* body = nullptr;
	long id;
private:
	REGISTER_START(Collider2DScriptBase);
};