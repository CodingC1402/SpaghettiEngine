#include "PlayerScoreBoard.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "SMath.h"
#include "SceneManager.h"
#include "Canvas.h"

REGISTER_FINISH(PlayerScoreBoard, ScriptBase) {}

void PlayerScoreBoard::OnStart()
{
	auto canvas = GET_FIRST_SCRIPT_OF_TYPE(Canvas);
	_scoreUI = dynamic_cast<UIString*>(canvas->GetComponent("Score"));
	_liveUI = dynamic_cast<UIString*>(canvas->GetComponent("Lives"));
}

void PlayerScoreBoard::OnUpdate()
{
	if (_score != _aimedScore)
	{
		_score += _delta * GameTimer::GetDeltaTime();
		if (_delta > 0)
			_score = _score > _aimedScore ? _aimedScore : _score;
		else
			_score = _score < _aimedScore ? _aimedScore : _score;

		if (_score >= _newLiveScore)
		{
			_gainedLive++;
			_newLiveScore += _newLive * _gainedLive;
			_live++;
		}
	}

	if (_live >= 0)
		_liveUI->SetText(std::to_string(_live));
	else
		_liveUI->SetText(std::to_string(0));
	_scoreUI->SetText(std::to_string(static_cast<unsigned>(_score)));
}

void PlayerScoreBoard::Load(nlohmann::json& input)
{
	_minDelta = input[Fields::PlayerScoreBoard::_minDelta].get<unsigned>();
	_increaseRate = input[Fields::PlayerScoreBoard::_increaseRate].get<float>();
	_live = input[Fields::PlayerScoreBoard::_live].get<unsigned>();
	_newLive = input[Fields::PlayerScoreBoard::_newLive].get<unsigned>();
	_newLiveScore = _newLive;
}

void PlayerScoreBoard::OnEnabled()
{
	__instance = this;
}

void PlayerScoreBoard::OnDisabled()
{
	__instance = __instance == this ? nullptr : __instance;
}

void PlayerScoreBoard::IncreaseScore(unsigned value)
{
	SetScore(_aimedScore + value);
}

void PlayerScoreBoard::SetScore(unsigned score)
{
	_aimedScore = score;
	_delta = _aimedScore - _score;
	_delta *= _increaseRate;
	_delta = SMath::Abs(_delta) < _minDelta ? (_minDelta * (_delta < 0 ? -1 : 1)) : _delta;
}

void PlayerScoreBoard::DecreaseLive()
{
	_live--;
	if (_live < 0)
		SceneManager::CallReloadScene();
}

int PlayerScoreBoard::GetLive() const noexcept
{
	return _live;
}

PlayerScoreBoard* PlayerScoreBoard::GetInstance()
{
	return __instance;
}
