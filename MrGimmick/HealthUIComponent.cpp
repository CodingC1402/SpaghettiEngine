#include "HealthUIComponent.h"
#include "Graphics.h"
#include "Texture.h"

REGISTER_UI_COMPONENT_FINISH(HealthUIComponent);
constexpr unsigned HEALTH_COUNT = 4;

constexpr auto MAXHEALTH_SPRITES = "MaxHealthSprites";
constexpr auto HEALTH_SPRITES = "HealthSprites";
constexpr auto TEXTURE = "Texture";
constexpr auto INDEX = "Index";
constexpr auto WIDTH = "Width";
constexpr auto SPACING = "Spacing";
constexpr auto HEALTH = "Health";
constexpr auto MAX_HEALTH = "MaxHealth";

void HealthUIComponent::Draw(Vector3 pos)
{
	auto actualPos = GetPosition() + pos;
	for (unsigned i = 0; i < _health; i++)
	{
		Graphics::DrawUI(_healthSprite[i], actualPos, GetColor());
		actualPos.x += _width + _spacing;
	}
	for (unsigned i = _health; i < _maxHealth; i++)
	{
		Graphics::DrawUI(_maxHealthSprite[i], actualPos, GetColor());
		actualPos.x += _width + _spacing;
	}
	UIComponent::Draw(pos);
}

void HealthUIComponent::Load(nlohmann::json& input)
{
	UIComponent::Load(input);
	_width = input[WIDTH].get<float>();
	_spacing = input[SPACING].get<float>();
	_maxHealth = input[MAX_HEALTH].get<unsigned>();
	_health = input[HEALTH].get<unsigned>();

	auto texture = TextureContainer::GetInstance()->GetResource(input[TEXTURE].get<CULL>());
	for (auto& sprite : input[HEALTH_SPRITES])
	{
		_healthSprite.push_back(texture->GetSprite(sprite[INDEX].get<unsigned>()));
	}
	for (auto& sprite : input[MAXHEALTH_SPRITES])
	{
		_maxHealthSprite.push_back(texture->GetSprite(sprite[INDEX].get<unsigned>()));
	}
}

void HealthUIComponent::SetMaxHealth(const unsigned& health) noexcept
{
	_maxHealth = health;
}

unsigned HealthUIComponent::GetMaxHealth() const noexcept
{
	return _maxHealth;
}

void HealthUIComponent::SetHealth(const unsigned& health) noexcept
{
	_health = health;
}

unsigned HealthUIComponent::GetHealth() const noexcept
{
	return _health;
}
