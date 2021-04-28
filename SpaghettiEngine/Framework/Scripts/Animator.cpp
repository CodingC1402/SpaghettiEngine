#include "Animator.h"
#include "GameTimer.h"

REGISTER_FINISH(Animator);
SAnimation* Animator::test;

Animator::Animator()
{
	name = TYPE_NAME(Animator);
}

void Animator::Update()
{
	time += GameTimer::GetDeltaTime();
	_ani->Advance(&frame, &time);
	sprite = _ani->GetSpriteOfFrame(&frame);
	SpriteRenderer::Update();
}

void Animator::Load(const nlohmann::json& inputObject)
{
	SpriteRenderer::Load(inputObject);
	try
	{
		_ani = Animation::GetAnimation(inputObject["AnimationPath"].get<std::string>());
		test = &_ani;
		frame = 0;
	}
	catch(std::exception e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this);
	}
}

void Animator::Unload()
{
	SpriteRenderer::Unload();
}
