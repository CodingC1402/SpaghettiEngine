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

void SpriteRenderer::Load(nlohmann::json& inputObject)
{
	ScriptBase::Load(inputObject);

	std::string fieldTracker = "Start of the script";
	try
	{
		constexpr const char* Path = "TexturePath";
		constexpr const char* Index = "Index";
		constexpr const char* OffSetX = "OffSetX";
		constexpr const char* OffSetY = "OffSetY";
		constexpr const char* ScaleX = "ScaleX";
		constexpr const char* ScaleY = "ScaleY";

		fieldTracker = Path;
		const auto texturePath = inputObject[Path].get<std::string>();
		fieldTracker = Index;
		const auto index = inputObject[Index].get<int>();
		fieldTracker = OffSetX;
		const auto offX = inputObject[OffSetX].get<float>();
		fieldTracker = OffSetY;
		const auto offY = inputObject[OffSetY].get<float>();
		fieldTracker = ScaleX;
		const auto scaleX = inputObject[ScaleX].get<float>();
		fieldTracker = ScaleY;
		const auto scaleY = inputObject[ScaleY].get<float>();
		
		STexture texture;
		Texture::GetTexture(&texture, texturePath);
		if (!texture->GetSprite(&sprite, index))
		{
			std::ostringstream os;
			os << "[Info] Index " << index << " of texture file " << texturePath.c_str()
				<< " is out of bound";
			fieldTracker += os.str();
		}
		transformMatrix._41 = offX;
		transformMatrix._42 = offY;
		transformMatrix._11 = scaleX;
		transformMatrix._22 = scaleY;
	}
	catch(const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, std::string("\n[Error field] ") + fieldTracker + "\n[Exception] " + e.what());
	}
}
