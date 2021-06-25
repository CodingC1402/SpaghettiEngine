#include "PlayerFeetScript.h"
#include "FieldNames.h"

REGISTER_FINISH(PlayerFeetScript);

PlayerFeetScript::PlayerFeetScript(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void PlayerFeetScript::OnStart()
{
	_moveScript = dynamic_cast<MoveScript*>(GetGameObject()->GetParent()->GetScriptContainer().GetItemType("MoveScript"));
}

void PlayerFeetScript::OnCollideEnter(CollideEvent& e)
{
	auto collideWith = e.GetGameObject().lock();
	if (!_moveScript->GetGrounded() && collideWith->GetTag() == Fields::Platform::_platform)
		_moveScript->SetGrounded(true);
}

void PlayerFeetScript::OnCollideExit(CollideEvent& e)
{
	auto collideWith = e.GetGameObject().lock();
	if (!_moveScript->GetGrounded() && collideWith->GetTag() == Fields::Platform::_platform)
		_moveScript->SetGrounded(false);
}

PScriptBase PlayerFeetScript::Clone() const
{
	auto clone = dynamic_cast<PlayerFeetScript*>(ScriptBase::Clone());

	clone->_isGrounded = _isGrounded;

	return PScriptBase();
}
