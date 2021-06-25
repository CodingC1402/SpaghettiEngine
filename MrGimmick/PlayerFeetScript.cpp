#include "PlayerFeetScript.h"
#include "FieldNames.h"

void PlayerFeetScript::Load(nlohmann::json& input)
{
}

void PlayerFeetScript::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
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
