#pragma once
#include "ScriptBase.h"
#include "PlayerInventory.h"

class Pickups : public ScriptBase
{
	using ItemType = PlayerInventory::ItemType;
public:
	void OnCollide(CollideEvent& e) override;
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	ItemType _type;
	bool _used = false;

	REGISTER_START(Pickups);
};

