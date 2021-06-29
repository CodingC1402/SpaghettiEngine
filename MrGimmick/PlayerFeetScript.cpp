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

	auto normal = e.GetNormal();
	if (collideWith->GetTag() == Fields::Platform::_platform && normal.y < 0)
		_isLastGrounded = true;
}

PScriptBase PlayerFeetScript::Clone() const
{
	auto clone = dynamic_cast<PlayerFeetScript*>(ScriptBase::Clone());

	clone->_isGrounded = _isGrounded;
	clone->_isLastGrounded = _isLastGrounded;

	return PScriptBase();
}
