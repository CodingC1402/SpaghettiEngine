#include "FeetScript.h"
#include "FieldNames.h"

REGISTER_FINISH(FeetScript);

FeetScript::FeetScript(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void FeetScript::OnStart()
{
	_moveScript = dynamic_cast<MoveScript*>(GetGameObject()->GetParent()->GetScriptContainer().GetItemType("MoveScript"));
}

void FeetScript::OnUpdate()
{

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

	auto normal = e.GetNormal();
	if (collideWith->GetTag() == Fields::Platform::_platform && normal.y < 0)
		_isLastGrounded = true;
}

PScriptBase FeetScript::Clone() const
{
	auto clone = dynamic_cast<FeetScript*>(ScriptBase::Clone());

	clone->_isGrounded = _isGrounded;
	clone->_isLastGrounded = _isLastGrounded;

	return PScriptBase();
}
