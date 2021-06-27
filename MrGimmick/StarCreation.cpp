#include "StarCreation.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "SMath.h"

REGISTER_FINISH(StarCreation);

StarCreation::StarCreation(PScene owner, bool isDisabled) : Render2DScriptBase(owner, isDisabled)
{}

void StarCreation::OnStart()
{
	_starScript = GET_FIRST_SCRIPT_OF_TYPE(StarScript);
}

void StarCreation::OnUpdate()
{
	_counter += GameTimer::GetDeltaTime();
	_currentSpinAngle += _spinAngle * GameTimer::GetDeltaTime();

	if (_counter >= _createTime)
	{
		_starScript->SetCreated();
		this->CallDestroy();
	}
	else
	{
		_radius = (1 - (_counter / _createTime)) * _baseRadius;
	}
}

void StarCreation::Draw(PCamera script)
{
	auto transform = script->GetMatrix(GetWorldMatrix());
	Graphics::SetSpriteTransform(transform);

	Vector3 position(0, _radius, 0);
	position = position * SMath::GetZAxisRotateMatrix(_currentSpinAngle);
	for (int i = 0; i < _numberOfStar; i++)
	{
		Graphics::DrawSprite(_currentSprite, Vector3(), position);
		position = position * _rotationMatrix;
	}
}

void StarCreation::Load(nlohmann::json& input)
{
	using Fields::Star;

	_createTime		= input[Star::_createTime].get<float>();
	_starAnim		= AnimationContainer::GetInstance()->GetResource(input[Star::_smallStarAnim].get<CULL>());
	_numberOfStar	= input[Star::_numberOfStar].get<CULL>();
	_spinAngle		= input[Star::_spinAngle].get<float>();
	_baseRadius		= input[Star::_radius].get<float>();

	_rotationMatrix = SMath::GetZAxisRotateMatrix(360.0f / _numberOfStar);

	Render2DScriptBase::Load(input);
}

PScriptBase StarCreation::Clone() const
{
	auto clone = dynamic_cast<StarCreation*>(Render2DScriptBase::Clone());

	clone->_createTime = _createTime;
	clone->_starAnim = _starAnim;
	clone->_currentSprite = _currentSprite;

	return clone;
}
