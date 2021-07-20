#pragma once
#include "ScriptBase.h"

class Enemy : public ScriptBase
{
public:
	void OnStart() override;
	void OnCollide(CollideEvent& e) override;

	void SetIsInTube();
	[[nodiscard]] bool IsInTube();
private:
	bool _isInTube = false;
	Tag _collideIgnore;
	REGISTER_START(Enemy);
};

