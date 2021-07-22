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

	// Call when took damage
	void AddToHealthEvent(const std::function<void(const int&, const int&)>& lamda);
	// Call when stop iFrame
	void AddToIFrameEvent(const std::function<void(const int&)>& lamda);
	void ClearHealthEvent() noexcept;
	void ClearIFrameEvent() noexcept;

	[[nodiscard]] int GetHealth() const noexcept;
	[[nodiscard]] int GetMaxHealth() const noexcept;
	[[nodiscard]] float GetIFrame() const noexcept;
	[[nodiscard]] bool IsInIFrame() const noexcept;

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
	std::list<std::function<void(const int&, const int&)>> _healthDelegates;
	std::list<std::function<void(const int&)>> _iFrameDelegates;

	REGISTER_START(HealthScript);
};

