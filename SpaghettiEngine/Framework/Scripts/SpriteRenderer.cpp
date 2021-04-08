#include "SpriteRenderer.h"
#include "CornException.h"

REGISTER_FINISH(SpriteRenderer);

SpriteRenderer::SpriteRenderer()
{
	tranformMatrix._11 = 1;
	tranformMatrix._22 = 1;
	tranformMatrix._33 = 1;
	tranformMatrix._44 = 1;
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
	tranformMatrix._41 = std::stoi(inputArg[OffSetX]);
	tranformMatrix._42 = std::stoi(inputArg[OffSetY]);
	tranformMatrix._11 = std::stoi(inputArg[ScaleX]);
	tranformMatrix._22 = std::stoi(inputArg[ScaleY]);
}

void SpriteRenderer::Update()
{

}
