#pragma once
#include "ScriptBase.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(PlayerScript);

class PlayerScript : public ScriptBase
{
public:
	void OnEnabled() override;
	void OnDisabled() override;

	void Respawn();
 
	[[nodiscard]] static GameObj* GetCurrentPlayer();
private:
	static inline unsigned _score = 0;
	static inline unsigned _live = 100;

	static inline WPlayerScript __currentInstance;
	REGISTER_START(PlayerScript);
};