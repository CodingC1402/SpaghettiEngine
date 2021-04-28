#include "SpriteRenderer.h"
#include "CornException.h"
#include "StringConverter.h"
#include "GraphicsMath.h"
#include "Setting.h"

REGISTER_FINISH(SpriteRenderer);

SpriteRenderer::SpriteRenderer()
{
	transformMatrix._11 = 1;
	GraphicsMath::ZeroMatrix(&transformMatrix);

	transformMatrix._11 = 1;
	transformMatrix._22 = 1;
	transformMatrix._33 = 1;
	transformMatrix._44 = 1;

	name = TYPE_NAME(SpriteRenderer);
}

Matrix SpriteRenderer::GetSpriteMatrix() const noexcept
{
	return transformMatrix;
}

#pragma region Get
SSprite SpriteRenderer::GetSprite()  const noexcept
{
	return sprite;
}
Vector3 SpriteRenderer::GetCenter() const noexcept
{
	return sprite->GetCenter();
}
PDx9Texture SpriteRenderer::GetTexture() const noexcept
{
	return sprite->GetSource()->GetImage();
}
RECT SpriteRenderer::GetSourceRect() const noexcept
{
	return sprite->GetSourceRect();
}
#pragma endregion 

bool SpriteRenderer::Copy(CPScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	const auto copyScript = dynamic_cast<const SpriteRenderer*>(script);
	this->transformMatrix = copyScript->transformMatrix;
	this->sprite = copyScript->sprite;
	return true;
}

void SpriteRenderer::Draw(SpriteHandler handler, PCamera camera)
{
	RECT srcRect = GetSourceRect();
	Vector3 center = GetCenter();
	Matrix transform = camera->GetMatrix(GetWorldMatrix());
	if (Setting::IsWorldPointPixelPerfect())
	{
		transform._41 = std::round(transform._41);
		transform._42 = std::round(transform._42);
	}

	handler->SetTransform(&transform);
	handler->Draw(
		GetTexture(),
		&srcRect,
		&center,
		nullptr,
		WHITE
	);
}

void SpriteRenderer::Load(const nlohmann::json& inputObject)
{
	ScriptBase::Load(inputObject);
	try
	{
		constexpr const char* Path = "TexturePath";
		constexpr const char* Index = "Index";
		constexpr const char* OffSetX = "OffSetX";
		constexpr const char* OffSetY = "OffSetY";
		constexpr const char* ScaleX = "ScaleX";
		constexpr const char* ScaleY = "ScaleY";

		const auto texturePath = inputObject[Path].get<std::string>();
		const auto index = inputObject[Index].get<int>();
		const auto offX = inputObject[OffSetX].get<float>();
		const auto offY = inputObject[OffSetY].get<float>();
		const auto scaleX = inputObject[ScaleX].get<float>();
		const auto scaleY = inputObject[ScaleY].get<float>();
		
		STexture texture;
		Texture::GetTexture(&texture, texturePath);
		if (!texture->GetSprite(&sprite, index))
		{
			std::wostringstream os;
			os << L"Index " << index << L" of texture file " << texturePath.c_str()
				<< L" is out of bound";
			throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
		}
		transformMatrix._41 = offX;
		transformMatrix._42 = offY;
		transformMatrix._11 = scaleX;
		transformMatrix._22 = scaleY;
	}
	catch (CornException& e)
	{
		std::wostringstream os;
		os << L"Input of sprite renderer script is in the wrong format\n";
		os << L"[Error] " << e.What();
		if (owner != nullptr)
		{
			os << L"[Path] ";
			os << owner->GetPath().c_str();
		}
	}
	catch(...)
	{
		throw SCRIPT_FORMAT_EXCEPT(this);
	}
}
