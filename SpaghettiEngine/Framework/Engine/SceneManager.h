#pragma once
#include "CornException.h"
#include "Scene.h"
#include <vector>

typedef class SceneManager* PSceneManager;

typedef class Scene* PScene;
typedef std::shared_ptr<Scene> SScene;
typedef std::unique_ptr<Scene> UScene;

typedef class GameObj* PGameObj;

class SceneManager
{
	friend class Game;
public:
	class SceneManagerException : public CornDiscriptionException
	{
	public:
		SceneManagerException(int line, const char* file, std::wstring discription) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static void LoadNextScene();
	static void LoadScene(int index);
	static void LoadPreviousScene();

	static SScene& GetConstScene();

	static SScene& GetCurrentScene();
	static int GetCurrentSceneIndex();
	static int GetNumberOfScene();
protected:
	static PSceneManager GetInstance();

	SceneManager();
	void Update();
	void Load();
	void Init();
protected:
	int sceneIndex;

	std::vector<SScene> scenes;
	SScene constScene;
	SScene currentScene;

	static PSceneManager __instance;
};

#define SCENEMANAGER_EXCEPT(discription) SceneManager::SceneManagerException(__LINE__,__FILE__,discription)