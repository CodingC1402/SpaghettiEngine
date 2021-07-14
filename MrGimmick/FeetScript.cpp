#include "FeetScript.h"
#include "FieldNames.h"

REGISTER_FINISH(FeetScript, ScriptBase) {}

void FeetScript::OnStart()
{
	_moveScript = dynamic_cast<MoveScript*>(GetGameObject()->GetParent()->GetScriptContainer().GetItemType("MoveScript"));
}

void FeetScript::OnFixedUpdate()
{
	if (_isLastGrounded != _moveScript->GetGrounded())
	{
		_moveScript->SetGrounded(_isLastGrounded);
	}
	_isLastGrounded = false;
}

void FeetScript::OnCollide(CollideEvent& e)
{
	auto collideWith = e.GetGameObject();
	if (collideWith->GetTag().Collide(Fields::SpecialTag::GetPlatformTag()) && e.GetNormal().y < 0.25f)
		_isLastGrounded = true;
}

PScriptBase FeetScript::Clone() const
{
	auto clone = dynamic_cast<FeetScript*>(ScriptBase::Clone());

	clone->_isGrounded = _isGrounded;
	clone->_isLastGrounded = _isLastGrounded;

	return PScriptBase();
}
