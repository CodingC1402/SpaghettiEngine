#include "StarCreation.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(StarCreation);

StarCreation::StarCreation(PScene owner, bool isDisabled) : Render2DScriptBase(owner, isDisabled)
{}

void StarCreation::OnStart()
{
	_starScript = GET_FIRST_SCRIPT_OF_TYPE(StarScript);
	_currentSprite = _starAnim->GetSpriteOfFrame(0); // Get First frame
}

void StarCreation::OnUpdate()
{

	_counter += GameTimer::GetDeltaTime();

	// Control animation
	_animCounter += GameTimer::GetDeltaTime();
	auto oldFrame = _frame;
	_animEnded = _starAnim->Advance(_frame, _animCounter);
	if (oldFrame != _frame)
	{
		_currentSprite = _starAnim->GetSpriteOfFrame(_frame);
		if (_animEnded)
			_frame -= 2;
	}
	
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
		if (Setting::IsWorldPointPixelPerfect())
		{
			position.x = std::roundf(position.x);
			position.y = std::roundf(position.y);
		}

		Graphics::DrawSprite(_currentSprite, _currentSprite->GetCenter(), position);
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
	_radius			= _baseRadius; // To prevent glitch

	_rotationMatrix = SMath::GetZAxisRotateMatrix(360.0f / _numberOfStar);

	Render2DScriptBase::Load(input);
}

PScriptBase StarCreation::Clone() const
{
	auto clone = dynamic_cast<StarCreation*>(Render2DScriptBase::Clone());

	clone->_createTime		= _createTime;
	clone->_counter			= _counter;

	clone->_animCounter		= _animCounter;
	clone->_frame			= _frame;

	clone->_starAnim		= _starAnim;
	clone->_currentSprite	= _currentSprite;

	clone->_baseRadius		= _baseRadius;
	clone->_radius			= _baseRadius; // Intentional! NOT A MISTAKE! >:V

	clone->_spinAngle		= _spinAngle;
	clone->_currentSpinAngle= _currentSpinAngle;

	clone->_rotationMatrix	= _rotationMatrix;
	clone->_numberOfStar	= _numberOfStar;

	return clone;
}
