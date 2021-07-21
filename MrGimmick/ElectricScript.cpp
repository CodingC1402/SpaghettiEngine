#include "ElectricScript.h"
#include "PhysicCollide.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(ElectricScript, ScriptBase) {}

void ElectricScript::OnStart()
{
	_currentSprite = _elcAnim->GetSpriteOfFrame(_frame); // Get First frame
	auto _animatorList = GET_ALL_SCRIPTS_OF_TYPE(Animator);
	_movingAnimator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	for (auto ani : _animatorList)
	{
		//ani.get
	}

	_runningField = _movingAnimator->GetField<bool>("IsRunning");
}

void ElectricScript::OnFixedUpdate()
{
	if (!_runningField.lock()->GetValue())
	{
		_idleCounter += GameTimer::GetDeltaTime();
		auto oldFrame = _frame;
		_animEnded = _elcAnim->Advance(_frame, _idleCounter);
		if (oldFrame != _frame)
		{
			_currentSprite = _elcAnim->GetSpriteOfFrame(_frame);
			if (_animEnded)
				_frame--;
		}
	}
	else
		_currentSprite = nullptr;

	if (_currentStar.expired())
	{
		std::set<GameObj*> _objs;

		PhysicCollide::GetCollidedWithRectangle(GetGameObject(), _objs, Vector3(0, 0, 0), 48.0f, 48.0f, Tag("PlayerAttack"), PhysicCollide::FilterMode::Contain);

		if (!_objs.empty())
		{
			for (auto obj : _objs)
			{
				if (!obj->GetTag().Contain("Platform"))
				{
					_currentStar = std::dynamic_pointer_cast<GameObj>(obj->GetSharedPtr());
					obj->CallDestroy();
					break;
				}
			}
		}
	}

}

void ElectricScript::Load(nlohmann::json& input)
{
	_elcAnim = AnimationContainer::GetInstance()->GetResource(input["ElectricAnim"].get<CULL>());

	ScriptBase::Load(input);
}

PScriptBase ElectricScript::Clone() const
{
	auto clone = dynamic_cast<ElectricScript*>(ScriptBase::Clone());

	clone->_counter = _counter;

	clone->_elcAnim = _elcAnim;
	clone->_currentSprite = _currentSprite;

	return clone;
}