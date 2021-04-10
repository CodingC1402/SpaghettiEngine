#include "SpriteRenderer.h"
#include "CornException.h"

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

#define TexturePath 0
#define SpriteIndex 1
#define OffSetX 2
#define OffSetY 3
#define ScaleX 4
#define ScaleY 5
void SpriteRenderer::Load(const std::string* inputArg, int argS)
{
	STexture texture;
	Texture::GetTexture(texture, inputArg[TexturePath]);
	if (texture->GetSprite(sprite, std::stoi(inputArg[SpriteIndex])))
	{
		std::wostringstream os;
		os	<< L"Index " << inputArg[1].c_str() << L" of texture file " << inputArg[TexturePath].c_str()
			<< L" is out of bound";
		throw CORN_EXCEPT_WITH_DISCRIPTION(os.str());
	}
	transformMatrix._41 = std::stoi(inputArg[OffSetX]);
	transformMatrix._42 = std::stoi(inputArg[OffSetY]);
	transformMatrix._11 = std::stoi(inputArg[ScaleX]);
	transformMatrix._22 = std::stoi(inputArg[ScaleY]);
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
