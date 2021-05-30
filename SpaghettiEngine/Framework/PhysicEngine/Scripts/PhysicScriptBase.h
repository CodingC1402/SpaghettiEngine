#pragma once
#include "ScriptBase.h"

class PhysicScriptBase : public ScriptBase
{
public:
	PhysicScriptBase(PScene owner, bool isDisable = false);
	void AssignOwner(const PGameObj& owner) override;

	void OnDisabled() override;
	void OnEnabled() override;
};