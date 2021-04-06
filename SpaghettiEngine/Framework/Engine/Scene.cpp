#include "Scene.h"

Scene::Scene(std::string path)
	:
	path(path)
{}

bool Scene::Load()
{
	return false;
}

bool Scene::Unload()
{
	Sprite::ClearSceneSprite();
	Texture::ClearSceneTexture();
}
