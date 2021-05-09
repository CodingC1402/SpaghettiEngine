#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"
#include "BoxCollider2D.h"

class RigidBody2D;
typedef class Collider2DScriptBase* PCollider2DScriptBase;

typedef std::map<std::string, void* (*)(PCollider2DScriptBase, PCollider2DScriptBase)> ColliderTypes;


class ColliderFactory
{
protected:
	static ColliderTypes* GetMap();
private:
	static ColliderTypes* map;
};


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

	virtual bool IsCollided(BoxCollider2D block);
protected:
	Vector3 offSet;
	bool isTrigger = false;
	RigidBody2D* body = nullptr;
private:
	REGISTER_START(Collider2DScriptBase);
};