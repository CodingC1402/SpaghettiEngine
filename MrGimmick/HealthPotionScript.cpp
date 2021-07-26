#include "HealthPotionScript.h"
#include "HealthBarScript.h"

REGISTER_FINISH(HealthPotionScript, SpecialAttackScript) {}

void HealthPotionScript::OnStart()
{
	SpecialAttackScript::OnStart();
	GetAttackScript()->AddEvent(
		[&](bool isCanceled)
		{
			if (!isCanceled) {
				GetAttackScript()->Explode();
				auto script = HealthBarScript::GetInstance()->GetPlayerHealthScript();
				script->SetHealth(script->GetMaxHealth());
			}
		}
	);
}