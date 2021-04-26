#include "TileSpriteRenderer.h"
#include <GraphicsMath.h>

REGISTER_FINISH(TileSpriteRenderer);

TileSpriteRenderer::TileSpriteRenderer()
{
	transformMatrix._11 = 1;
	GraphicsMath::ZeroMatrix(&transformMatrix);

	transformMatrix._11 = 1;
	transformMatrix._22 = 1;
	transformMatrix._33 = 1;
	transformMatrix._44 = 1;

	name = TYPE_NAME(TileSpriteRenderer);
}

void TileSpriteRenderer::LoadTile(const std::string& texturePath, int index, int offSetX, int offSetY, int scaleX, int scaleY)
{
	std::string oX = std::to_string(offSetX);
	std::string oY = std::to_string(offSetY);
	std::string sX = std::to_string(scaleX);
	std::string sY = std::to_string(scaleY);
	std::string id = std::to_string(index);
	std::string arg = texturePath + " " + id + " " + oX + " " + oY + " " + sX + " " + sY + " ";
	Load(&arg);
}

int TileSpriteRenderer::GetSrcWidth() const
{
	return sprite->GetWidth();
}

int TileSpriteRenderer::GetSrcHeight() const
{
	return sprite->GetHeight();
}

