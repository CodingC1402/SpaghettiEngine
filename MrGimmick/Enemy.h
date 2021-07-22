#pragma once
#include "ScriptBase.h"

class Enemy : public ScriptBase
{
public:
	void OnCollide(CollideEvent& e) override;
	void Load(nlohmann::json& input) override;
	void SetIsInTube();

	ScriptBase* Clone() const override;
	[[nodiscard]] bool IsInTube();
private:
	bool _isInTube = false;
	Tag _collideIgnore;
	REGISTER_START(Enemy);
};

