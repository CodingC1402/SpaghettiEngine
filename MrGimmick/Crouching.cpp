#include "Crouching.h"
#include "PhysicCollide.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(Crouching, ScriptBase) {}

void Crouching::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
	_behaviorTree->AssignMoveScript(_moveScript);
	_behaviorTree->SetGameObject(GetGameObject());

	auto _animatorList = GET_ALL_SCRIPTS_OF_TYPE(Animator);

	for (auto ani : _animatorList)
	{
		if (ani->GetName() == "Moving")
			_movingAnimator = dynamic_cast<Animator*>(ani);
		if (ani->GetName() == "Electric")
			_electricAnimator = dynamic_cast<Animator*>(ani);
	}

	_defendField = _electricAnimator->GetField<bool>("IsDefend");
	_crouchingField = _movingAnimator->GetField<bool>("IsCrouching");
}

void Crouching::OnFixedUpdate()
{
	std::set<GameObj*> _objs;

	PhysicCollide::GetCollidedWithRectangle(GetGameObject(), _objs, Vector3(0, 16, 0), 16.0f, 10.0f, Tag("Player"), PhysicCollide::FilterMode::Contain);

	if (!_objs.empty())
	{
		for (auto obj : _objs)
		{
			_behaviorTree->StopMove();
			_crouchingField.lock()->SetValue(true);
		}

		if (!_defendField.lock()->GetValue())
		{
			_electricAnimator->Disable();
		}
	}
	else
	{
		_crouchingField.lock()->SetValue(false);
	}
}

void Crouching::Load(nlohmann::json& input)
{
	std::string _treeFilePath = input[Fields::AIScript::_behaviorTree].get<std::string>();

	_behaviorTree = MAKE_SHARE_BT(AIBTs);
	_behaviorTree->Load(_treeFilePath, input[Fields::AIScript::_changes]);

	ScriptBase::Load(input);
}

PScriptBase Crouching::Clone() const
{
	auto clone = dynamic_cast<Crouching*>(ScriptBase::Clone());

	return clone;
}