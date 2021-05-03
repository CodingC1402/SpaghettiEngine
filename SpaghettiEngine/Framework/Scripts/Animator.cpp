#include "Animator.h"
#include "GameTimer.h"
#include "Setting.h"

REGISTER_FINISH(Animator);

Animator::Animator(PScene owner)
	:
	Render2DScriptBase(owner)
{
	_name = TYPE_NAME(Animator);
}

void Animator::OnUpdate()
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

Scene::BaseComponent* Animator::Clone()
{
	throw CORN_EXCEPT_WITH_DESCRIPTION(L"Unimplemented function");
	return nullptr;
}
