#include "StarScript.h"
#include "GameTimer.h"

REGISTER_FINISH(StarScript);

StarScript::StarScript(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void StarScript::OnEnabled()
{
	originalPos = GetWorldTransform();
}

void StarScript::OnUpdate()
{
	if (onWayBack)
	{
		if (GetWorldTransform().x < originalPos.x)
		{
			onWayBack = false;
		}
		else
		{
			_ownerObj->Translate(Vector3(-20, 0, 0) * GameTimer::GetDeltaTime());
		}
	}
	else
	{
		if (GetWorldTransform().x > originalPos.x + 400)
		{
			onWayBack = true;
		}
		else
		{
			_ownerObj->Translate(Vector3(20, 0, 0) * GameTimer::GetDeltaTime());
		}
	}
}

void StarScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject().lock()->GetTag() == "Player")
		e.SetIsHandled(true);
}

SScriptBase StarScript::Clone() const
{
	auto clone = std::dynamic_pointer_cast<StarScript>(ScriptBase::Clone());

	clone->onWayBack = onWayBack;
	clone->originalPos = originalPos;

	return clone;
}
