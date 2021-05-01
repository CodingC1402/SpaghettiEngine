#include "Animator.h"
#include "GameTimer.h"
#include "Setting.h"

REGISTER_FINISH(Animator);

Animator::Animator()
{
	name = TYPE_NAME(Animator);
}

void Animator::Update()
{
	time += GameTimer::GetDeltaTime();
	_ani->Advance(frame, time);
}

void Animator::Draw(SpriteHandler handler, PCamera camera)
{
	_sprite = _ani->GetSpriteOfFrame(frame);
	RECT srcRect = _sprite->GetSourceRect();
	Vector3 center = _sprite->GetCenter();
	Matrix transform = camera->GetMatrix(GetWorldMatrix());
	if (Setting::IsWorldPointPixelPerfect())
	{
		transform._41 = std::round(transform._41);
		transform._42 = std::round(transform._42);
	}

	handler->SetTransform(&transform);
	handler->Draw(
		_sprite->GetSource()->GetImage(),
		&srcRect,
		&center,
		nullptr,
		WHITE
	);
}

void Animator::Load(nlohmann::json& inputObject)
{
	try
	{
		_ani = AnimationContainer::GetInstance()->GetResource(inputObject["Animation"].get<CULL>());
		frame = 0;
	}
	catch(const std::exception&)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, "");
	}
	Render2DScriptBase::Load(inputObject);
}