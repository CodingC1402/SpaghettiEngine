#include "SpecialAttackScript.h"
#include "AttackScript.h"
#include "PlayerInventory.h"

REGISTER_FINISH(SpecialAttackScript, ScriptBase) {}

void SpecialAttackScript::OnStart()
{
	auto script = GET_FIRST_SCRIPT_OF_TYPE(AttackScript);
	script->AddEvent(
		[&](bool isCanceled)
		{
			if (!isCanceled) {
				PlayerInventory::GetInstance()->RemoveTopItem();
			}
		}
	);
	_attackScript = script;
}

AttackScript* SpecialAttackScript::GetAttackScript() const noexcept
{
	return _attackScript;
}
