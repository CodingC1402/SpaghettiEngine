#include "SpriteRenderer.h"
#include "CornException.h"
#include "Graphics.h"
#include "StringConverter.h"
#include "SMath.h"
#include "Setting.h"
#include "ScriptField.h"

REGISTER_FINISH(SpriteRenderer, Render2DScriptBase)
{
	transformMatrix._11 = 1;

	transformMatrix._11 = 1;
	transformMatrix._22 = 1;
	transformMatrix._33 = 1;
	transformMatrix._44 = 1;
}

Matrix4 SpriteRenderer::GetSpriteMatrix() const noexcept
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
	Vector3 center = GetCenter();
	Matrix4 transform = camera->GetMatrix(transformMatrix * GetWorldMatrix());
	Graphics::SetSpriteTransform(transform);
	Graphics::DrawSprite(sprite, GetCenter());
}

void SpriteRenderer::Load(nlohmann::json& inputObject)
{
	std::string fieldTracker = "Start of the script";
	try
	{
		using Fields::SpriteRenderer;
		fieldTracker = SpriteRenderer::GetPathField();
		const auto textureID = inputObject[SpriteRenderer::GetPathField()].get<CULL>();
		fieldTracker = SpriteRenderer::GetIndexField();
		const auto index = inputObject[SpriteRenderer::GetIndexField()].get<int>();

		transformMatrix._41 = inputObject[SpriteRenderer::GetOffSetXField()].empty() ? 0 : inputObject[SpriteRenderer::GetOffSetXField()].get<float>();
		transformMatrix._42 = inputObject[SpriteRenderer::GetOffSetYField()].empty() ? 0 : inputObject[SpriteRenderer::GetOffSetYField()].get<float>();
		transformMatrix._11 = inputObject[SpriteRenderer::GetScaleXField() ].empty() ? 1 : inputObject[SpriteRenderer::GetScaleXField() ].get<float>();
		transformMatrix._22 = inputObject[SpriteRenderer::GetScaleYField() ].empty() ? 1 : inputObject[SpriteRenderer::GetScaleYField() ].get<float>();

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

PScriptBase SpriteRenderer::Clone() const
{
	auto clone = dynamic_cast<SpriteRenderer*>(ScriptBase::Clone());

	clone->transformMatrix = transformMatrix;
	clone->sprite = sprite;

	return clone;
}
