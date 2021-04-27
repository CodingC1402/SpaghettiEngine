#pragma once

#include "CornWnd.h"
#include <string>
#include <list>
#include <memory>
#include <json.hpp>

typedef class GameObj* PGameObj;
typedef class TileObj* PTileObj;
typedef class Scene* PScene;
typedef std::unique_ptr<Scene> UScene;
typedef std::shared_ptr<Scene> SScene;

using namespace nlohmann;
class Scene
{
	friend class SceneManager;
	friend class GameObj;
public:
	void Update();
	PGameObj GetObj(UINT index[],UINT size);
protected:
	Scene(std::string path);
	void Instantiate(PGameObj gameObj);
	void RemoveGameObject(PGameObj gameObj);
	void AddGameObject(PGameObj gameObj);
	void Load();
	void Unload();
protected:
	std::string path = "";

	std::list<PGameObj> instances;
};