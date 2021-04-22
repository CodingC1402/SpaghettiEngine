#include "SpriteRenderer.h"
#include "CornException.h"
#include "StringConverter.h"
#include "GraphicsMath.h"

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

#pragma region Get
Matrix SpriteRenderer::GetTransform()  const noexcept
{
	return transformMatrix;
}
SSprite SpriteRenderer::GetSprite()  const noexcept
{
	return sprite;
}
Vector3 SpriteRenderer::GetPosition() const noexcept
{
	return owner->GetWorldTransform();
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

bool SpriteRenderer::Copy(const PScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	SpriteRenderer* copyScript = static_cast<SpriteRenderer*>(script);
	this->transformMatrix = copyScript->transformMatrix;
	this->sprite = copyScript->sprite;
	return true;
}

void SpriteRenderer::Update()
{
	Graphics::Draw(this);
}

void SpriteRenderer::Draw(const SpriteHandler& handler, const Matrix& cameraMatrix, const bool& isPixelPerfect)
{
	handler->SetTransform(&(owner->GetWorldMatrix() * cameraMatrix));
	handler->Draw(
		GetTexture(),
		&GetSourceRect(),
		&GetCenter(),
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
