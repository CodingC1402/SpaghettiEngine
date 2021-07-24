#pragma once
#include "ScriptBase.h"
#include "UIString.h"

class PlayerScoreBoard : public ScriptBase
{
public:
	void OnStart() override;
	void OnUpdate() override;
	void Load(nlohmann::json& input) override;

	void OnEnabled() override;
	void OnDisabled() override;

	void SetScore(unsigned score);
	void DecreaseLive();
	[[nodiscard]] int GetLive() const noexcept;

	[[nodiscard]] static PlayerScoreBoard* GetInstance();
private:
	int _live = 0;

	unsigned _score = 0;
	unsigned _aimedScore = 0;
	unsigned _minDelta = 0;

	UIString* _scoreUI = nullptr;
	UIString* _liveUI = nullptr;

	// percent per second
	float _increaseRate = 0;

	static inline PlayerScoreBoard* __instance = nullptr;
	REGISTER_START(PlayerScoreBoard);
};

