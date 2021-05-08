#include "SpriteRenderer.h"
#include "CornException.h"
#include "Graphics.h"
#include "StringConverter.h"
#include "GraphicsMath.h"
#include "Setting.h"

REGISTER_FINISH(SpriteRenderer);

SpriteRenderer::SpriteRenderer(PScene owner) : Render2DScriptBase(owner)
{
	transformMatrix._11 = 1;
	GraphicsMath::ZeroMatrix(&transformMatrix);

	transformMatrix._11 = 1;
	transformMatrix._22 = 1;
	transformMatrix._33 = 1;
	transformMatrix._44 = 1;

	_name = TYPE_NAME(SpriteRenderer);
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
PTexture SpriteRenderer::GetTexture() const noexcept
{
	return sprite->GetSource();
}
RECT SpriteRenderer::GetSourceRect() const noexcept
{
	return sprite->GetSourceRect();
}
#pragma endregion 

void SpriteRenderer::Draw(PCamera camera)
{
	RECT srcRect = GetSourceRect();
	Vector3 center = GetCenter();
	Matrix transform = camera->GetMatrix(transformMatrix * GetWorldMatrix());
	Graphics::SetSpriteTransform(transform);
	Graphics::DrawSprite(sprite, GetCenter());
}

void SpriteRenderer::Load(nlohmann::json& inputObject)
{
	std::string fieldTracker = "Start of the script";
	try
	{
		constexpr const char* Path = "Texture";
		constexpr const char* Index = "Index";
		constexpr const char* OffSetX = "OffSetX";
		constexpr const char* OffSetY = "OffSetY";
		constexpr const char* ScaleX = "ScaleX";
		constexpr const char* ScaleY = "ScaleY";
		
		fieldTracker = Path;
		const auto textureID = inputObject[Path].get<CULL>();
		fieldTracker = Index;
		const auto index = inputObject[Index].get<int>();

		transformMatrix._41 = inputObject[OffSetX] == nullptr ? 0 : inputObject[OffSetX].get<float>();
		transformMatrix._42 = inputObject[OffSetY] == nullptr ? 0 : inputObject[OffSetY].get<float>();
		transformMatrix._11 = inputObject[ScaleX] == nullptr ? 1 : inputObject[ScaleX].get<float>();
		transformMatrix._22 = inputObject[ScaleY] == nullptr ? 1 : inputObject[ScaleY].get<float>();

		fieldTracker = Path;
		STexture texture = TextureContainer::GetInstance()->GetResource(textureID);
		sprite = texture->GetSprite(index);
		if (sprite.use_count() == 0)
		{
			std::ostringstream os;
			os << "[Info] Index " << index << " of texture file " << textureID
				<< " is out of bound";
			fieldTracker += os.str();
		}
	}
	catch(const CornException& e)
	{
		std::wostringstream os;
		os << L"Error field " << fieldTracker.c_str() << "\n\n" << e.What();
		std::wstring w = e.What();
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	catch(const std::exception& e)
	{
		std::wostringstream os;
		os << "\n[Error field] " << fieldTracker.c_str() << "\n[Exception] " << e.what();
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	Render2DScriptBase::Load(inputObject);
}