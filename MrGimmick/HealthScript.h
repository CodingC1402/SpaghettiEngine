#pragma once
#include "ScriptBase.h"
#include "CRectangle.h"
#include <functional>
#include <list>

class HealthScript : public ScriptBase
{
public:
	void OnUpdate() override;
	void OnFixedUpdate() override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;

	void AddToEvent(const std::function<void(const int&, const int&)>& lamda);
	void ClearEvent();

	[[nodiscard]] int GetHealth() const noexcept;
	[[nodiscard]] int GetMaxHealth() const noexcept;
	[[nodiscard]] float GetIFrame() const noexcept;

	void SetHealth(int health) noexcept;
	void SetMaxHealth(int health) noexcept;
	void SetIFrame(float frame) noexcept;
private:
	int _health = 0;
	int _maxHealth = 0;

	float _iFrame = 0;
	float _coolDown = 0;

	std::vector<CRectangle> _hitBoxes;
	std::vector<std::pair<Tag, unsigned>> _damageSources;
	Tag _masterTag;
	// Max health and damage
	std::list<std::function<void(const int&, const int&)>> _delegates;

	REGISTER_START(HealthScript);
};

