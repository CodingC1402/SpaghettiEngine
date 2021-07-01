#include "PlayerHead.h"
#include "FieldNames.h"

REGISTER_FINISH(PlayerHead);

PlayerHead::PlayerHead(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{}

void PlayerHead::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE_FROM_PARENT(MoveScript);
}

void PlayerHead::OnFixedUpdate()
{
	_moveScript->SetAllowJump(!_isCollideWithPlatform);
	_isCollideWithPlatform = false;
}

void PlayerHead::OnCollide(CollideEvent& e)
{
	// \...../ - Check if the shape collide with is above this.
	//  \.../
	//   \A/

	if (e.GetNormal().y >= 0.0f && e.GetGameObject()->GetTag() == Fields::Platform::_platform)
	{
		_isCollideWithPlatform = true;
		_moveScript->ResetJumpAction();
	}
}

PScriptBase PlayerHead::Clone() const
{
	auto clone = dynamic_cast<PlayerHead*>(ScriptBase::Clone());

	clone->_isCollideWithPlatform = _isCollideWithPlatform;

	return clone;
}
