#include "FireBallScript.h"
#include "FieldNames.h"

REGISTER_FINISH(FireBallScript, SpecialAttackScript) {}

void FireBallScript::OnStart()
{
	SpecialAttackScript::OnStart();
	_script = GET_FIRST_SCRIPT_OF_TYPE(AttackScript);
}

void FireBallScript::OnCollide(CollideEvent& e)
{
	e.SetIsHandled(true);
	if (e.GetGameObject()->GetTag() == Fields::SpecialTag::GetPlatformTag())
	{
		_script->Explode();
	}
}
