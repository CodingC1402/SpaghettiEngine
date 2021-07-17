#include "RidableScript.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(RidableScript, ScriptBase) {}

void RidableScript::OnStart()
{
	_lastPosition = GetGameObject()->GetTransform().GetTransform();
}

void RidableScript::OnUpdate()
{
	if (_isStand)
	{
		Vector3 currentPosition = GetGameObject()->GetTransform().GetTransform();
		Vector3 tempVec = (currentPosition - _lastPosition);

		auto& transform = _ridingObj->GetTransform();
		transform.Translate(tempVec);
	}
	_isStand = false;
	_lastPosition = GetGameObject()->GetTransform().GetTransform();
}

void RidableScript::OnFixedUpdate()
{
	
}

void RidableScript::OnCollide(CollideEvent& e)
{
	auto collideWith = e.GetGameObject();
	
	auto normal = e.GetNormal();
	if (collideWith->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()) && normal.y > 0)
	{
		_isStand = true;
		_moveScript = dynamic_cast<MoveScript*>(collideWith->GetParent()->GetScriptContainer().GetItemType("MoveScript"));
		_ridingObj = collideWith->GetParent();
	}
}

PScriptBase RidableScript::Clone() const
{
	return PScriptBase();
}