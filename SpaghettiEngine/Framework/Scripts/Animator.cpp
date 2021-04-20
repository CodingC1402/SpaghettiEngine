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

void Animator::Load(const std::string* inputArg)
{
	_ani = Animation::GetAnimation(inputArg);
	test = &_ani;
	frame = 0;
}

void Animator::Unload()
{
	SpriteRenderer::Unload();
	_ani.reset();
}
