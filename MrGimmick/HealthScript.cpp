#include "HealthScript.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "PhysicCollide.h"
#include "Setting.h"
#include "DebugRenderer.h"

REGISTER_FINISH(HealthScript, ScriptBase) {}
constexpr unsigned HITBOX_COLOR = 0xFFFF0000;

void HealthScript::OnUpdate()
{
	_coolDown -= GameTimer::GetDeltaTime();
}

void HealthScript::OnFixedUpdate()
{
	if (_coolDown > 0)
		return;

	std::set<GameObj*> collideSet;
	for (auto& rect : _hitBoxes)
	{
		PhysicCollide::GetCollidedWithRectangle(
			GetGameObject(),
			collideSet,
			rect.GetCenter(),
			rect.GetWidth(),
			rect.GetHeight(),
			_masterTag,
			PhysicCollide::FilterMode::Collide
		);

		if constexpr (Setting::IsDebugMode())
		{
			DebugRenderer::DrawRectangleFromCenter(
				rect.GetCenter(),
				rect.GetWidth(),
				rect.GetHeight(),
				GetWorldMatrix(),
				HITBOX_COLOR
			);
		}
	}
	if (collideSet.empty())
		return;

	for (auto& source : _damageSources)
	{
		for (auto& obj : collideSet)
		{
			if (obj->GetTag().Contain(source.first))
			{
				_coolDown = _iFrame;
				_health -= source.second;
				for (auto& fun : _delegates)
					fun(_health, -static_cast<int>(source.second));
				break;
			}
		}
		if (_coolDown > 0)
			return;
	}
}

void HealthScript::Load(nlohmann::json& input)
{
	_maxHealth = input[Fields::HealthScript::_maxHealth].get<unsigned>();
	_health = input[Fields::HealthScript::_health].get<unsigned>();
	_health = _health > _maxHealth ? _maxHealth : _health;
	_iFrame = input[Fields::HealthScript::_iFrame].get<int>();
	for (auto& source : input[Fields::HealthScript::_damageSource])
	{
		_damageSources.push_back(std::pair(Tag(source[Fields::HealthScript::_tag]), source[Fields::HealthScript::_damage]));
		_masterTag = _masterTag | _damageSources.back().first;
	}
	for (auto& hitbox : input[Fields::HealthScript::_hitBoxes])
	{
		_hitBoxes.push_back(CRectangle(
			hitbox[Fields::HealthScript::_center],
			hitbox[Fields::HeadScript::_width].get<float>(),
			hitbox[Fields::HeadScript::_height].get<float>()
		));
	}
}

ScriptBase* HealthScript::Clone() const
{
	auto clone = dynamic_cast<HealthScript*>(ScriptBase::Clone());

	clone->_maxHealth = _maxHealth;
	clone->_health = _health;
	clone->_iFrame = _iFrame;
	clone->_coolDown = _coolDown;
	clone->_damageSources = _damageSources;

	return clone;
}

void HealthScript::AddToEvent(const std::function<void(const int&, const int&)>& lamda)
{
	_delegates.push_back(lamda);
}

void HealthScript::ClearEvent()
{
	_delegates.clear();
}

int HealthScript::GetHealth() const noexcept
{
	return _health;
}

int HealthScript::GetMaxHealth() const noexcept
{
	return _maxHealth;
}

float HealthScript::GetIFrame() const noexcept
{
	return _iFrame;
}

void HealthScript::SetHealth(int health) noexcept
{
	health = health > _maxHealth ? _maxHealth : health;
	health = health < 0 ? 0 : health;

	auto delta = health - _health;
	if (delta == 0)
		return;
	_health = health;

	for (auto& fun : _delegates)
		fun(_health, delta);
}

void HealthScript::SetMaxHealth(int health) noexcept
{
	health = health < 0 ? 0 : health;
	_maxHealth = health;
	if (_maxHealth < _health)
		SetHealth(_maxHealth);
}

void HealthScript::SetIFrame(float frame) noexcept
{
	_iFrame = frame;
}
