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

void SpriteRenderer::Load(const std::string* inputArg)
{
	try
	{
		constexpr int TexturePath = 0;
		constexpr int OffSetX = 2;
		constexpr int OffSetY = 3;
		constexpr int ScaleX = 4;
		constexpr int ScaleY = 5;
		TokenizedStr input = StringConverter::Tokenize(inputArg, ' ');
		
		STexture texture;
		Texture::GetTexture(&texture, input[TexturePath]);
		if (constexpr int SpriteIndex = 1; !texture->GetSprite(&sprite, std::stoi(input[SpriteIndex])))
		{
			std::wostringstream os;
			os << L"Index " << input[1].c_str() << L" of texture file " << input[TexturePath].c_str()
				<< L" is out of bound";
			throw CORN_EXCEPT_WITH_DISCRIPTION(os.str());
		}
		transformMatrix._41 = std::stof(input[OffSetX]);
		transformMatrix._42 = std::stof(input[OffSetY]);
		transformMatrix._11 = std::stof(input[ScaleX]);
		transformMatrix._22 = std::stof(input[ScaleY]);
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
}
