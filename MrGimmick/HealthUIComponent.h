#pragma once
#include "UIComponent.h"
#include "Sprite.h"

class HealthUIComponent : public UIComponent
{
public:
	void Draw(Vector3 pos) override;
	void Load(nlohmann::json& input) override;

	void SetMaxHealth(const unsigned& health) noexcept;
	[[nodiscard]] unsigned GetMaxHealth() const noexcept;

	void SetHealth(const unsigned& health) noexcept;
	[[nodiscard]] unsigned GetHealth() const noexcept;
private:
	// 1 -> 4
	unsigned _maxHealth = 0;
	unsigned _health = 0;
	float _width = 0;
	float _spacing = 0;

	std::vector<SSprite> _healthSprite;
	std::vector<SSprite> _maxHealthSprite;

	REGISTER_UI_COMPONENT_START(HealthUIComponent);
};

