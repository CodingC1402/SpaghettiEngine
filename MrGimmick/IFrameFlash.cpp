#include "IFrameFlash.h"
#include "FieldNames.h"
#include "GameTimer.h"

REGISTER_FINISH(IFrameFlash, ScriptBase) {}

void IFrameFlash::OnStart()
{
	_animator = GET_FIRST_SCRIPT_OF_TYPE(Animator);
	_healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	// To stop flash when iFrame stop
	_healthScript->AddToIFrameEvent([&](const int& health) {
		StopFlash();
	});
	// To start flash
	_healthScript->AddToHealthEvent([&](const int& health, const int& delta) {
		if (health > 0 && delta < 0)
			StartFlash();
	}); 
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
	_reducedAlpha = input[Fields::IFrameFlash::_flashAlpha].get<Byte>();
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

ScriptBase* IFrameFlash::Clone() const
{
	auto clone = dynamic_cast<IFrameFlash*>(ScriptBase::Clone());

	clone->_flashDelay = _flashDelay;
	clone->_reducedAlpha = _reducedAlpha;

	return clone;
}
