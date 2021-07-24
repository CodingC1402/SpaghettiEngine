#include "AttackMove.h"
#include "LoadingJson.h"
#include "FieldNames.h"
#include "PlayerSound.h"
#include "PhysicCollide.h"
#include "Setting.h"
#include "DebugRenderer.h"

REGISTER_FINISH(AttackMove, ScriptBase) {}
static constexpr auto DEBUG_COLOR = 0xFFFF0000;

void AttackMove::OnStart()
{
	_attackKey = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput("Attack"));
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
}

void AttackMove::OnUpdate()
{
	if (_attackKey->CheckKeyPress() && _currentStar.expired())
	{
		auto star = GetOwner()->Instantiate(_starPrefab, GetWorldTransform() + _appearOffSet);

		_currentStar = std::dynamic_pointer_cast<GameObj>(star->GetSharedPtr());
		_starScript = dynamic_cast<AttackScript*>(star->GetScriptContainer().GetItemType(TYPE_NAME(AttackScript)));
		_throwed = false;

		PlayerSound::GetCurrentPlayerSound()->PlayStarCreateSound();
	}
	if (_starScript && _attackKey->CheckKeyRelease())
	{
		_starScript->Throw(Vector3(), _moveScript->IsFlipped());
		_starScript = nullptr;
		_needPlatformCheck = true;
		_throwed = true;
	}
}

void AttackMove::OnFixedUpdate()
{
	if (_needPlatformCheck)
	{
		_needPlatformCheck = false;
		std::set<GameObj*> objList;
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			objList,
			_cancelZone.GetCenter(),
			_cancelZone.GetWidth(),
			_cancelZone.GetHeight(),
			Fields::SpecialTag::GetPlatformTag(),
			PhysicCollide::FilterMode::Equal
		);
		if (!objList.empty())
		{
			if (!_currentStar.expired())
			{
				_currentStar.lock()->GetTransform().SetWorldTransform(GetWorldTransform() + _appearOffSet);
			}
			ForceThrow();
		}
	}

	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangleFromCenter(
			_cancelZone.GetCenter(),
			_cancelZone.GetWidth(),
			_cancelZone.GetHeight(),
			GetWorldMatrix(),
			DEBUG_COLOR
		);
	}
}


void AttackMove::OnLateUpdate()
{
	if (_currentStar.expired() || _throwed)
		return;
	_currentStar.lock()->GetTransform().SetWorldTransform(GetWorldTransform() + _appearOffSet);
}

void AttackMove::OnDisabled()
{
	ForceThrow();
}

void AttackMove::ForceThrow()
{
	if (_starScript)
	{
		_starScript->Throw(_rb->GetVelocity(), _moveScript->IsFlipped());
		_starScript->Explode();
	}
	else if (!_currentStar.expired())
	{
		_starScript = dynamic_cast<AttackScript*>(_currentStar.lock()->GetScriptContainer().GetItemType(TYPE_NAME(AttackScript)));
		_starScript->Explode();
	}
	_starScript = nullptr;
}

void AttackMove::Load(nlohmann::json& input)
{
	_starPrefab = dynamic_cast<GameObj*>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][LoadingJson::Field::idField].get<CULL>()));
	auto offSet = input[Fields::Player::_appearOffSet];
	_appearOffSet.x = offSet[0].get<float>();
	_appearOffSet.y = offSet[1].get<float>();
	_appearOffSet.z = offSet[2].get<float>();
	_cancelZone = input[Fields::Star::_cancelZone];
}
