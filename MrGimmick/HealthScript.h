#pragma once
#include "ScriptBase.h"

// Just for testing
#include "SoundSource.h"
#include "InputSystem.h"
#include "InputAll.h"
typedef std::shared_ptr<InputAll> SInputAll;
//

class HealthScript : public ScriptBase
{
public:
	void Load(nlohmann::json& input);
	void OnStart() override;
	void OnUpdate() override;

	void OnCollide(CollideEvent& e) override;

	void SetHealth(int delta);
	void AddHealth(int delta);

	void SetMaxHealth(int delta);
	void AddMaxHealth(int delta);
	
	void Heal(int hp);
	void TakeDamage(int hp);
	void OutOfHealth();

	PScriptBase Clone() const override;
protected:
	std::map<Tag, int> healthPointMap;
	void (*func)() = nullptr;
	int currentHealth;
	int maxHealth;
	float currentTime = 0;
	float delayTime = 1.0f;
	bool isTakingDamage = false;

	// Just for testing
	SInputAll _damageInput;
	SInputAll _healInput;
	//

	SoundSource* _audio = nullptr;

private:
	REGISTER_START(HealthScript);
};