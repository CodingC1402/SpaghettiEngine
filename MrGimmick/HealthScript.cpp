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
		currentHealth = maxHealth;
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
	if (isTakingDamage)
		return;

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
