#include "SpriteRenderer.h"
#include "CornException.h"
#include "StringConverter.h"

REGISTER_FINISH(SpriteRenderer);

SpriteRenderer::SpriteRenderer()
{
	transformMatrix._11 = 1;
	transformMatrix._22 = 1;
	transformMatrix._33 = 1;
	transformMatrix._44 = 1;

	name = TYPE_NAME(SpriteRenderer);
}

bool SpriteRenderer::Copy(const PScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	SpriteRenderer* copyScript = static_cast<SpriteRenderer*>(script);
	this->transformMatrix = copyScript->transformMatrix;
	this->sprite = copyScript->sprite;
	return true;
}

Matrix& SpriteRenderer::GetTransform()
{
	return transformMatrix;
}

SSprite& SpriteRenderer::GetSprite()
{
	return sprite;
}

Vector3 SpriteRenderer::GetPosition()
{
	Vector3 position = owner->GetPosition();
	position.x -= sprite->GetWidth() / 2;
	position.y += sprite->GetHeight() / 2;
	return position;
}

PDx9Texture SpriteRenderer::GetTexture()
{
	return sprite->GetSource()->GetImage();
}

RECT SpriteRenderer::GetSourceRect()
{
	return sprite->GetSourceRect();
}

void SpriteRenderer::Update()
{
	Graphics::Draw(this);
}

#define TexturePath 0
#define SpriteIndex 1
#define OffSetX 2
#define OffSetY 3
#define ScaleX 4
#define ScaleY 5

void SpriteRenderer::Load(const std::string* inputArg)
{
	try
	{
		TokenizedStr input = StringConverter::Tokenize(inputArg, ' ');
		STexture texture;
		Texture::GetTexture(&texture, input[TexturePath]);
		if (!texture->GetSprite(&sprite, std::stoi(input[SpriteIndex])))
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
		throw e;
	}
}
