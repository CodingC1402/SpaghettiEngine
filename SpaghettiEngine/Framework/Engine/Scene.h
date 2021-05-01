#pragma once

#include "CornWnd.h"
#include "CornException.h"
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
	class SceneException : public CornException
	{
	public:
		SceneException(int line, const char* file, const std::string& description);
		const wchar_t* GetType() const noexcept override;
		const wchar_t* What() const noexcept override;
	protected:
		std::string _description;
	};
public:
	void Start();
	void Update();
	void End();
	PGameObj GetObj(UINT index[],UINT size);
protected:
	Scene(const std::string& path);
	void Instantiate(PGameObj gameObj);
	void RemoveGameObject(PGameObj gameObj);
	void AddGameObject(PGameObj gameObj);
	void Load();
	void Unload();
protected:
	std::string path;

	std::list<PGameObj> instances;
};

#define SCENE_EXCEPTION(description) Scene::SceneException(__LINE__,__FILE__,description)