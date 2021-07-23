#include "DropBombScript.h"
#include "PhysicCollide.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "FieldNames.h"
#include "DebugRenderer.h"
#include "PlayerScript.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(DropBombScript, ScriptBase) {}
static constexpr unsigned DEBUG_COLOR = 0xFFFF0000;
static constexpr unsigned FANSPEED_ZONE0 = 0x7700FF00;
static constexpr unsigned FANSPEED_ZONE1 = 0x77FFFF00;
static constexpr unsigned FANSPEED_ZONE2 = 0x77FF0000;

void DropBombScript::OnStart()
{
	auto scripts = GET_ALL_SCRIPTS_OF_TYPE(Polygon2DCollider);
	for (auto& script : scripts)
	{
		if (script->GetName() == "AfterDropCollider")
			_afterExplodeCollider = dynamic_cast<Polygon2DCollider*>(script);
		else
			_polyCollider = dynamic_cast<Polygon2DCollider*>(script);
	}
	_rbBody = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);

	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_explodedField = _animator->GetField<bool>("IsExploded");
	_dropField = _animator->GetField<bool>("IsDroped");
	_fanSpeed = _animator->GetField<unsigned>("FanSpeed");
}

void DropBombScript::OnFixedUpdate()
{
	std::set<GameObj*> _objs;

	if (_rbBody->IsDisabled() && !_polyCollider->IsDisabled())
	{
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			_objs,
			_rect.GetCenter(),
			_rect.GetWidth(),
			_rect.GetHeight(),
			Tag("Player"),
			PhysicCollide::FilterMode::Collide
		);
		auto disVec = PlayerScript::GetCurrentPlayer()->GetTransform().GetWorldTransform() - GetWorldTransform();
		disVec.y = 0;
		float distanceToPlayer = disVec.GetMagnitude();
		int fanSpeed = (_totalFanLevel - 1) - static_cast<int>(distanceToPlayer / _levelRadius);
		_fanSpeed.lock()->SetValue(fanSpeed < 0 ? 0 : fanSpeed);

		if constexpr (Setting::IsDebugMode())
		{
			DebugRenderer::DrawRectangleFromCenter(
				_rect.GetCenter(),
				_rect.GetWidth(),
				_rect.GetHeight(),
				GetWorldMatrix(),
				DEBUG_COLOR
			);

			switch (fanSpeed)
			{
			case 0:
				DebugRenderer::DrawCircle(_levelRadius, GetWorldMatrix(), FANSPEED_ZONE0);
				break;
			case 1:
				DebugRenderer::DrawCircle(_levelRadius, GetWorldMatrix(), FANSPEED_ZONE1);
				break;
			case 2:
				DebugRenderer::DrawCircle(_levelRadius, GetWorldMatrix(), FANSPEED_ZONE2);
				break;
			}

		}
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
	if (e.GetGameObject()->GetTag().Contain(Tag(Fields::SpecialTag::GetPlayerAttack())) && _currentStar.expired())
	{
		_currentStar = std::dynamic_pointer_cast<GameObj>(e.GetGameObject()->GetSharedPtr());
		e.GetGameObject()->CallDestroy();
	}
	else if (e.GetGameObject()->GetTag() == Fields::SpecialTag::GetPlatformTag() || e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		_rbBody->Disable();
		_polyCollider->Disable();
		_afterExplodeCollider->Enable();
		_explodedField.lock()->SetValue(true);
		_isStarted = true;
	}
}

void DropBombScript::Load(nlohmann::json& input)
{
	_rect.SetWidth(input[Fields::DropBombScript::_width].get<float>());
	_rect.SetHeight(input[Fields::DropBombScript::_height].get<float>());
	_rect.SetCenter(input[Fields::DropBombScript::_center]);

	_levelRadius = input[Fields::DropBombScript::_levelRadius].get<float>();
	_totalFanLevel = input[Fields::DropBombScript::_totalFanLevel].get<unsigned>();

	_explodeTime = input[Fields::DropBombScript::_explodeTime].get<float>();
}

PScriptBase DropBombScript::Clone() const
{
	auto clone = dynamic_cast<DropBombScript*>(ScriptBase::Clone());

	clone->_rect = _rect;

	clone->_levelRadius = _levelRadius;
	clone->_totalFanLevel = _totalFanLevel;
	clone->_explodeTime = _explodeTime;

	return clone;
}
