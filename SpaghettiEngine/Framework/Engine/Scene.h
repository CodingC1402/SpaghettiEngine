#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Texture.h"
#include "Sprite.h"

typedef class Scene* PScene;
typedef std::shared_ptr<Scene> SScene;
typedef std::unique_ptr<Scene> UScene;

class Scene
{
	friend class SceneManager;
protected:
	Scene(std::string path);
	bool Load();
	bool Unload();
protected:
	std::string path;
};