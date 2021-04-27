#include "TileObj.h"


TileObj::TileObj(const TileObj& obj) : GameObj(obj)
{
	this->ownerScene = obj.ownerScene;
}

TileObj::TileObj(const std::string& path, const PScene& ownerScene) : GameObj(path, ownerScene)
{
}

void TileObj::AddTileSpriteRenderer(std::string texturePath, int index , int posX, int posY, int offSetX, int offSetY, int scaleX, int scaleY)
{
	TileSpriteRenderer* tileSprite = new TileSpriteRenderer();
	tileSprite->LoadTile(texturePath, index);
	tileSprite->owner = this;
	AddScript(tileSprite);
	int px = posX * tileSprite->GetSrcWidth();
	int py = posY * tileSprite->GetSrcHeight();
	SetTransform(px, py, 0);
}

