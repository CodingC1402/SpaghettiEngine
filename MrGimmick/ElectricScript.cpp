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
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
	_behaviorTree->AssignMoveScript(_moveScript);
	_behaviorTree->SetGameObject(GetGameObject());

	auto _animatorList = GET_ALL_SCRIPTS_OF_TYPE(Animator);

	for (auto ani : _animatorList)
	{
		if (ani->GetName() == "Electric")
			_electricAnimator = dynamic_cast<Animator*>(ani);
	}
	
	_defendField = _electricAnimator->GetField<bool>("IsDefend");
}

void ElectricScript::OnUpdate()
{
	if (_counterStart)
		_counter += GameTimer::GetDeltaTime();

	_time += GameTimer::GetDeltaTime();

	if (_counter > _animationTime)
	{
		_defendField.lock()->SetValue(false);
		_counterStart = false;
		_counter = 0;
		_time = 0;
	}
}

void ElectricScript::OnFixedUpdate()
{
	if (_moveScript->IsWalking())
	{
		_time = 0;
		_electricAnimator->Disable();
	}
	else
	{
		if (_time > _waitTime)
		{
			_time = 0;

			if (_electricAnimator->IsDisabled())
			{
				_defendField.lock()->SetValue(false);
				_electricAnimator->Enable();
			}
			else
				_electricAnimator->Disable();
		}
	}

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
					_behaviorTree->StopMove();
					_counterStart = true;
					_counter = 0;
					_time = 0;
					_electricAnimator->Enable();

					_defendField.lock()->SetValue(true);
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
	std::string _treeFilePath = input[Fields::AIScript::_behaviorTree].get<std::string>();
	_animationTime = input["AnimationTime"].get<float>();

	_behaviorTree = MAKE_SHARE_BT(AIBTs);
	_behaviorTree->Load(_treeFilePath, input[Fields::AIScript::_changes]);

	ScriptBase::Load(input);
}

PScriptBase ElectricScript::Clone() const
{
	auto clone = dynamic_cast<ElectricScript*>(ScriptBase::Clone());

	clone->_counter = _counter;

	return clone;
}