#include "HealthScript.h"
#include "GameTimer.h"
#include "FieldNames.h"

REGISTER_FINISH(HealthScript, ScriptBase) {}

void HealthScript::Load(nlohmann::json& input)
{
	constexpr const char* MaxHealth = "MaxHealth";
	constexpr const char* Delay = "Delay";
	constexpr const char* HealthSource = "HealthSource";
	constexpr const char* TagName = "Tag";
	constexpr const char* HealthPoint = "HealthPoint";

	try
	{
		maxHealth = input[MaxHealth].get<int>();
		delayTime = input[Delay].get<float>();

		for (auto j : input[HealthSource])
		{
			healthPointMap.insert(std::make_pair(Tag(j[TagName]), j[HealthPoint].get<int>()));
		}
	}
	catch (const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, StringConverter::StrToWStr(e.what()));
	}
	ScriptBase::Load(input);
}

void HealthScript::OnStart()
{
	// Just for testing
	_audio = GET_FIRST_SCRIPT_OF_TYPE(SoundSource);

	_damageInput = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_jump));
	_healInput = std::dynamic_pointer_cast<InputAll>(InputSystem::GetInput(Fields::Input::_attack));
	//
}

void HealthScript::OnUpdate()
{
	if (isTakingDamage)
		currentTime += GameTimer::GetDeltaTime();

	if (currentTime > delayTime)
	{
		currentTime = 0;
		isTakingDamage = false;
	}

	// Just for testing
	if (_damageInput->CheckKeyDown())
		TakeDamage(1.0f);
	else if (_healInput->CheckKeyDown())
		Heal(1.0f);
	//
}

void HealthScript::OnCollide(CollideEvent& e)
{
	bool doHave = false;
	Tag collide = e.GetGameObject()->GetTag();
	auto it = healthPointMap.begin();

	for (it = healthPointMap.begin(); it != healthPointMap.end(); it++)
	{
		if (it->first.Collide(collide))
		{
			doHave = true;
			break;
		}
	}

	if (!doHave)
		return;

	if (it->second > 0)
	{
		Heal(it->second);
	} 
	else if (it->second < 0)
	{
		TakeDamage(it->second);
	}

	if (currentHealth == 0)
	{
		OutOfHealth();
	}
}

void HealthScript::SetHealth(int delta)
{
	currentHealth = delta;

	if (currentHealth < 0)
		currentHealth = 0;
	
	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}

void HealthScript::AddHealth(int delta)
{
	currentHealth += delta;

	if (currentHealth < 0)
		currentHealth = 0;

	if (currentHealth > maxHealth)
		currentHealth = maxHealth;
}

void HealthScript::SetMaxHealth(int delta)
{
	maxHealth = delta;

	if (maxHealth < 0)
		maxHealth = 0;
}

void HealthScript::AddMaxHealth(int delta)
{
	maxHealth += delta;

	if (maxHealth < 0)
		maxHealth = 0;
}

void HealthScript::Heal(int hp)
{
	AddHealth(hp);

	// Just for testing
	_audio->Play();
	//
}

void HealthScript::TakeDamage(int hp)
{
	AddHealth(hp);
	isTakingDamage = true;

	// Just for testing
	_audio->Play();
	//
}

void HealthScript::OutOfHealth()
{
	// Just for testing
	_audio->Play();
	//
}

PScriptBase HealthScript::Clone() const
{
	return PScriptBase();
}
