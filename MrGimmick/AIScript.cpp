#include "AIScript.h"
#include "FieldNames.h"
#include "Physic.h"
#include "MoveScript.h"

REGISTER_FINISH(AIScript, ScriptBase) {}

void AIScript::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
	_behaviorTree->AssignMoveScript(_moveScript);
	_behaviorTree->SetGameObject(GetGameObject());
}

void AIScript::OnFixedUpdate()
{
	_accumulate += Physic::GetStep();
	if (_accumulate >= _delay)
	{
		_behaviorTree->SetDeltaTime(_accumulate);
		_behaviorTree->Tick();
		_accumulate = 0;
	}
}

void AIScript::Load(nlohmann::json& input)
{
	std::string _treeFilePath = input[Fields::AIScript::_behaviorTree].get<std::string>();
	_delay = input[Fields::AIScript::_delay].get<float>();

	_behaviorTree = MAKE_SHARE_BT(AIBTs);
	_behaviorTree->Load(_treeFilePath);
}

ScriptBase* AIScript::Clone() const
{
	auto clone = dynamic_cast<AIScript*>(ScriptBase::Clone());

	clone->_delay = _delay;
	clone->_accumulate = _accumulate;
	clone->_behaviorTree = std::dynamic_pointer_cast<AIBTs>(_behaviorTree->Clone());

	return clone;
}
