#include "Crouching.h"
#include "PhysicCollide.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(Crouching, ScriptBase) {}
static constexpr unsigned DEBUG_COLOR = 0xFFFF0000;

void Crouching::OnStart()
{
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
	auto _animatorList = GET_ALL_SCRIPTS_OF_TYPE(Animator);

	for (auto ani : _animatorList)
	{
		if (ani->GetName() == Fields::ElectricEnemy::_moving)
			_movingAnimator = dynamic_cast<Animator*>(ani);
		if (ani->GetName() == Fields::ElectricEnemy::_electric)
			_electricAnimator = dynamic_cast<Animator*>(ani);
	}

	_defendField = _electricAnimator->GetField<bool>(Fields::ElectricEnemy::_isDefend);
	_crouchingField = _movingAnimator->GetField<bool>(Fields::ElectricEnemy::_isCrouching);
}

void Crouching::OnFixedUpdate()
{
	std::set<GameObj*> _objs;

	PhysicCollide::GetCollidedWithRectangle(GetGameObject(), _objs, 
		_rect.GetCenter(),
		_rect.GetWidth(),
		_rect.GetHeight(), Fields::SpecialTag::GetPlayerTag(), PhysicCollide::FilterMode::Contain);

	if (!_objs.empty())
	{
		for (auto obj : _objs)
		{
			_moveScript->StopMoveLeft();
			_moveScript->StopMoveRight();
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

	if constexpr (Setting::IsDebugMode())
	{
		DebugRenderer::DrawRectangleFromCenter(
			_rect.GetCenter(),
			_rect.GetWidth(),
			_rect.GetHeight(),
			GetWorldMatrix(),
			DEBUG_COLOR);
	}
}

void Crouching::Load(nlohmann::json& input)
{
	_rect.SetWidth(input[Fields::ElectricEnemy::_width].get<float>());
	_rect.SetHeight(input[Fields::ElectricEnemy::_height].get<float>());
	_rect.SetCenter(input[Fields::ElectricEnemy::_center]);

	ScriptBase::Load(input);
}

PScriptBase Crouching::Clone() const
{
	auto clone = dynamic_cast<Crouching*>(ScriptBase::Clone());

	clone->_rect = _rect;

	return clone;
}