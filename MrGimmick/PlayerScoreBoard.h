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

	void IncreaseScore(unsigned value);
	void SetScore(unsigned score);
	void DecreaseLive();
	[[nodiscard]] int GetLive() const noexcept;

	[[nodiscard]] static PlayerScoreBoard* GetInstance();
private:
	unsigned _minDelta = 0;

	UIString* _scoreUI = nullptr;
	UIString* _liveUI = nullptr;

	// percent per second
	static inline float _increaseRate = 0;
	static inline float _score = 0;
	static inline float _delta = 0;
	static inline unsigned _newLive = 0;
	static inline unsigned _newLiveScore = 0;
	static inline unsigned _gainedLive = 1;
	static inline int _live = 0;

	unsigned _aimedScore = 0;

	static inline PlayerScoreBoard* __instance = nullptr;
	REGISTER_START(PlayerScoreBoard);
};

