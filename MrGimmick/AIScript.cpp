#include "AIScript.h"
#include "FieldNames.h"
#include "Physic.h"

REGISTER_FINISH(AIScript, ScriptBase) {}

void AIScript::OnFixedUpdate()
{
	_accumulate += Physic::GetStep();
	if (_accumulate >= _delay)
	{
		_behaviralTree->Tick();
		_accumulate = 0;
	}
}

void AIScript::Load(nlohmann::json& input)
{
	std::string _treeFilePath = input[Fields::AIScript::_behaviorTree].get<std::string>();

	_behaviralTree = MAKE_SHARE_BT(AIBTs);
	_behaviralTree->Load(_treeFilePath);
}
