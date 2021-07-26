#pragma once
#include "ScriptBase.h"
#include "PlayerInventory.h"

class Pickups : public ScriptBase
{
	using ItemType = PlayerInventory::ItemType;
public:
	void OnUpdate() override;
	void OnCollide(CollideEvent& e) override;
	void Load(nlohmann::json& input) override;
	void SetTime(unsigned time);
	ScriptBase* Clone() const override;
private:
	ItemType _type;
	bool _used = false;
	float _time = 0;

	REGISTER_START(Pickups);
};

