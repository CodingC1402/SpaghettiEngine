#include "PlayerFeetScript.h"
#include "FieldNames.h"

REGISTER_FINISH(PlayerFeetScript);

PlayerFeetScript::PlayerFeetScript(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void PlayerFeetScript::OnStart()
{
	_moveScript = dynamic_cast<MoveScript*>(GetGameObject()->GetParent()->GetScriptContainer().GetItemType("MoveScript"));
}

void PlayerFeetScript::OnUpdate()
{

}

void PlayerFeetScript::OnFixedUpdate()
{
	if (_isLastGrounded != _moveScript->GetGrounded())
	{
		_moveScript->SetGrounded(_isLastGrounded);
	}
	_isLastGrounded = false;
}

void PlayerFeetScript::OnCollide(CollideEvent& e)
{
	auto collideWith = e.GetGameObject();
	if (collideWith->GetTag() == Fields::Platform::_platform)
		_isLastGrounded = true;
}

void PlayerFeetScript::OnCollideEnter(CollideEvent& e)
{
	//auto collideWith = e.GetGameObject();
	//if (!_moveScript->GetGrounded() && collideWith->GetTag() == Fields::Platform::_platform)
	//	_moveScript->SetGrounded(true);
}

void PlayerFeetScript::OnCollideExit(CollideEvent& e)
{
	//auto collideWith = e.GetGameObject();
	//if (_moveScript->GetGrounded() && collideWith->GetTag() == Fields::Platform::_platform)
	//	_moveScript->SetGrounded(false);
}

PScriptBase PlayerFeetScript::Clone() const
{
	auto clone = dynamic_cast<PlayerFeetScript*>(ScriptBase::Clone());

	clone->_isGrounded = _isGrounded;

	return PScriptBase();
}
