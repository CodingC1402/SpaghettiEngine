#pragma once
#include "ScriptBase.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(PlayerScript);

class PlayerScript : public ScriptBase
{
public:
	void OnEnabled() override;
	void OnDisabled() override;
 
	[[nodiscard]] static GameObj* GetCurrentPlayer();
private:
	unsigned _score;

	static inline WPlayerScript __currentInstance;
	REGISTER_START(PlayerScript);
};

