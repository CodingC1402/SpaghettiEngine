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
	if (coounterStarted)
	{
		counter += GameTimer::GetDeltaTime();
		if (counter >= explodeTime)
			GetGameObject()->CallDestroy();
	}

	if (onWayBack)
	{
		if (GetWorldTransform().x < originalPos.x)
		{
			onWayBack = false;
		}
		else
		{
			GetGameObject()->GetTransform().Translate(Vector3(-20, 0, 0) * GameTimer::GetDeltaTime());
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
			GetGameObject()->GetTransform().Translate(Vector3(20, 0, 0) * GameTimer::GetDeltaTime());
		}
	}
}

void StarScript::StartCounter()
{
	coounterStarted = true;
}

void StarScript::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject().lock()->GetTag() == "Player")
		e.SetIsHandled(true);
}

PScriptBase StarScript::Clone() const
{
	auto clone = dynamic_cast<StarScript*>(ScriptBase::Clone());

	clone->onWayBack = onWayBack;
	clone->originalPos = originalPos;

	return clone;
}
