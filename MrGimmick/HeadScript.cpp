#include "HeadScript.h"
#include "FieldNames.h"

REGISTER_FINISH(HeadScript);

// save compute time, doesn't need to recreate tag

HeadScript::HeadScript(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void HeadScript::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE_FROM_PARENT(MoveScript);
}

void HeadScript::OnFixedUpdate()
{
	_moveScript->SetAllowJump(!_isCollideWithPlatform);
	_isCollideWithPlatform = false;
}

void HeadScript::OnCollide(CollideEvent& e)
{
	// \...../ - Check if the shape collide with is above this.
	//  \.../
	//   \A/

	if (e.GetNormal().y >= 0.25f && e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlatformTag()))
	{
		_isCollideWithPlatform = true;
		_moveScript->ResetJumpAction();
	}
}

PScriptBase HeadScript::Clone() const
{
	auto clone = dynamic_cast<HeadScript*>(ScriptBase::Clone());

	clone->_isCollideWithPlatform = _isCollideWithPlatform;

	return clone;
}
