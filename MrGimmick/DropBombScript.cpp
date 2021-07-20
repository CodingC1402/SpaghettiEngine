#include "DropBombScript.h"
#include "PhysicCollide.h"

REGISTER_FINISH(DropBombScript, ScriptBase) {}

void DropBombScript::OnStart()
{
}

void DropBombScript::OnUpdate()
{
	std::set<GameObj*> _objs;

	if (_rbBody->IsDisabled())
		PhysicCollide::GetCollidedWithRectangle(GetGameObject(), _objs, Vector3(0, 0, 0), 100.0f, 100.0f, Tag("Player"), PhysicCollide::FilterMode::Contain);

	if (!_objs.empty())
	{
		_counter = 0;
	}
}

void DropBombScript::Load(nlohmann::json& input)
{
}

PScriptBase DropBombScript::Clone() const
{
	return PScriptBase();
}
