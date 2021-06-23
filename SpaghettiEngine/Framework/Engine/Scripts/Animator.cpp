#include "Animator.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "Setting.h"

REGISTER_FINISH(Animator);

Animator::Animator(PScene owner)
	:
	Render2DScriptBase(owner)
{}

void Animator::OnUpdate()
{
	time += GameTimer::GetDeltaTime();
	_ani->Advance(frame, time);
}

void Animator::Draw(PCamera camera)
{
	_sprite = _ani->GetSpriteOfFrame(frame);
	Matrix4 transform = camera->GetMatrix(GetWorldMatrix());
	Graphics::SetSpriteTransform(transform);
	Graphics::DrawSprite(_sprite, _sprite->GetCenter());
}

PScriptBase Animator::Clone() const
{
	auto animClone = dynamic_cast<Animator*>(Render2DScriptBase::Clone());

	animClone->_ani = _ani;
	animClone->_sprite = _sprite;
	animClone->frame = frame;
	animClone->time = time;

	return animClone;
}

void Animator::Load(nlohmann::json& inputObject)
{
	try
	{
		_ani = AnimationContainer::GetInstance()->GetResource(inputObject["Animation"].get<CULL>());
		frame = 0;
	}
	catch (const CornException& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, e.What());
	}
	catch(const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, StringConverter::StrToWStr(e.what()));
	}
	Render2DScriptBase::Load(inputObject);
}
