#pragma once
#include "ScriptBase.h"
#include "Macros.h"
#include "Animator.h"
#include "PlayerControl.h"
#include "MoveScript.h"
#include "RigidBody2D.h"

CLASS_FORWARD_DECLARATION(PlayerScript);

class PlayerScript : public ScriptBase
{
public:
	void OnStart() override;
	void OnEnabled() override;
	void OnDisabled() override;
	void OnUpdate() override;

	void TookDamage(const int& health, const int& damage);
	void Respawn();
 
	void Load(nlohmann::json& input);
	[[nodiscard]] static GameObj* GetCurrentPlayer();
private:
	BoolField _isHurted;

	float _hurtTime = 0;
	float _hurtCounter = 0;
	Vector3 _hurtVel;

	PlayerControl* _control = nullptr;
	MoveScript* _moveScript = nullptr;
	RigidBody2D* _rb = nullptr;

	static inline unsigned _score = 0;
	static inline unsigned _live = 100;

	static inline WPlayerScript __currentInstance;
	REGISTER_START(PlayerScript);
};