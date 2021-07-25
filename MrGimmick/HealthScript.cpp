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
	if (_coolDown > 0)
	{
		_coolDown -= GameTimer::GetDeltaTime();
		if (_coolDown <= 0)
		{
			for (auto& fun : _iFrameDelegates)
				fun(GetHealth());
		}
	}
}

void HealthScript::OnFixedUpdate()
{
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

	bool tookDamage = false;
	for (auto& source : _damageSources)
	{
		if (_coolDown <= 0 || source._ignoreIFrame)
		{
			for (auto& obj : collideSet)
			{
				if (obj->GetTag().Contain(source._tag))
				{
					_coolDown = _iFrame;
					_health -= source._damage;
					tookDamage = true;
					for (auto& fun : _healthDelegates)
						fun(_health, -static_cast<int>(source._damage));
					break;
				}
			}
			if (tookDamage)
				return;
		}
	}
}

void HealthScript::Load(nlohmann::json& input)
{
	_maxHealth = input[Fields::HealthScript::_maxHealth].get<unsigned>();
	_health = input[Fields::HealthScript::_health].get<unsigned>();
	_health = _health > _maxHealth ? _maxHealth : _health;
	_iFrame = input[Fields::HealthScript::_iFrame].get<float>();
	for (auto& source : input[Fields::HealthScript::_damageSource])
	{
		_damageSources.push_back(DamageSource(
			Tag(source[Fields::HealthScript::_tag]), 
			source[Fields::HealthScript::_damage].get<unsigned>(),
			source[Fields::HealthScript::_ignoreIFrame].empty() ? false : source[Fields::HealthScript::_ignoreIFrame].get<bool>()
		));
		_masterTag = _masterTag | _damageSources.back()._tag;
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
	clone->_masterTag = _masterTag;
	clone->_hitBoxes = _hitBoxes;

	return clone;
}

void HealthScript::AddToHealthEvent(const std::function<void(const int&, const int&)>& lamda)
{
	_healthDelegates.push_back(lamda);
}

void HealthScript::AddToIFrameEvent(const std::function<void(const int&)>& lamda)
{
	_iFrameDelegates.push_back(lamda);
}

void HealthScript::ClearHealthEvent() noexcept
{
	_healthDelegates.clear();
}

void HealthScript::ClearIFrameEvent() noexcept
{
	_iFrameDelegates.clear();
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

bool HealthScript::IsInIFrame() const noexcept
{
	return _coolDown > 0;
}

void HealthScript::SetHealth(int health) noexcept
{
	health = health > _maxHealth ? _maxHealth : health;
	health = health < 0 ? 0 : health;

	auto delta = health - _health;
	if (delta == 0)
		return;
	_health = health;

	for (auto& fun : _healthDelegates)
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

HealthScript::DamageSource::DamageSource(const Tag& tag, const unsigned& damage, bool ignoreIFrame)
{
	_tag = tag;
	_damage = damage;
	_ignoreIFrame = ignoreIFrame;
}
