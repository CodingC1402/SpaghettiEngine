#include "TileMapObj.h"

TileMapObj::TileMapObj(const TileMapObj& obj) : GameObj(obj)
{
}

TileMapObj::TileMapObj(const std::string& path, const PScene& ownerScene) : GameObj(path, ownerScene)
{
}
