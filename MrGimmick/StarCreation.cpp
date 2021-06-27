#include "StarCreation.h"
#include "GameTimer.h"

void StarCreation::OnStart()
{
	_starScript = GET_FIRST_SCRIPT_OF_TYPE(StarScript);
}

void StarCreation::OnFixedUpdate()
{
	_counter += GameTimer::GetDeltaTime();
	if (_counter >= _createTime)
	{
		_starScript->SetCreated();
		this->CallDestroy();
	}
}

void StarCreation::Load(nlohmann::json& input)
{

	Render2DScriptBase::Load(input);
}

PScriptBase StarCreation::Clone() const
{
	auto clone = dynamic_cast<StarCreation*>(Render2DScriptBase::Clone());

	clone->_createTime = _createTime;
	clone->_starAnim = _starAnim;

	return clone;
}
