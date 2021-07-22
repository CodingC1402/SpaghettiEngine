#include "IFrameFlash.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(IFrameFlash, ScriptBase) {}

void IFrameFlash::OnStart()
{
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
}

void IFrameFlash::OnUpdate()
{
	if (_isRunning)
	{
		_flashDelayCounter += GameTimer::GetDeltaTime();
		while (_flashDelayCounter > _flashDelay)
		{
			_flashDelayCounter -= _flashDelay;
			if (_isFlashed)
				_animator->SetColor(WHITE);
			else
				_animator->SetColor(Color(255, 255, 255, _reducedAlpha));
			_isFlashed = !_isFlashed;
		}
	}
}

void IFrameFlash::Load(nlohmann::json& input)
{
	_flashDelay = input[Fields::IFrameFlash::_flashDelay].get<float>();
	_reducedAlpha = input[Fields::IFrameFlash::_flashAlpha].get<float>();
}

void IFrameFlash::StartFlash()
{
	_isRunning = true;
}

void IFrameFlash::StopFlash()
{
	_isRunning = false;
	_animator->SetColor(WHITE);
	_flashDelayCounter = 0;
	_isFlashed = false;
}
