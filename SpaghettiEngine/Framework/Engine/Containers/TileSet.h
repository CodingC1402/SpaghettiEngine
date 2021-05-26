#pragma once
#include "Texture.h"

class TileSet : Resource
{

};

class TileSetContainer : public Container<TileSet>
{
	friend class SceneManager;
public:
	TileSetContainer();
};