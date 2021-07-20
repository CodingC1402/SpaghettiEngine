#include "DropBombScript.h"
#include "PhysicCollide.h"
#include "GameTimer.h"

REGISTER_FINISH(DropBombScript, ScriptBase) {}

void DropBombScript::OnStart()
{
	_polyCollider = GET_FIRST_SCRIPT_OF_TYPE(Polygon2DCollider);
	_rbBody = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);

	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_explodedField = _animator->GetField<bool>("IsExploded");
	_dropField = _animator->GetField<bool>("IsDroped");
}

void DropBombScript::OnUpdate()
{
	std::set<GameObj*> _objs;

	if (_rbBody->IsDisabled() && !_polyCollider->IsDisabled())
	{
		PhysicCollide::GetCollidedWithRectangle(GetGameObject(), _objs, Vector3(0, 0, 0), _width, _height, Tag("Player"), PhysicCollide::FilterMode::Contain);
	}

	if (!_objs.empty())
	{
		_rbBody->Enable();
		_dropField.lock()->SetValue(true);
	}

	if (_isStarted)
		_counter += GameTimer::GetDeltaTime();

	if (_counter > _explodeTime)
		GetGameObject()->CallDestroy();
}

void DropBombScript::OnCollide(CollideEvent& e)
{
	_rbBody->Disable();
	_polyCollider->Disable();
	_explodedField.lock()->SetValue(true);
	_isStarted = true;
}

void DropBombScript::Load(nlohmann::json& input)
{
	_width = input["Width"].get<float>();
	_height = input["Height"].get<float>();
	_explodeTime = input["ExplodeTime"].get<float>();
}

PScriptBase DropBombScript::Clone() const
{
	return PScriptBase();
}
