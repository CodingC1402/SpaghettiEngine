#pragma once
#include "CornException.h"
#include "Scene.h"
#include <vector>

typedef class SceneManager* PSceneManager;
typedef class GameObj* PGameObj;

class SceneManager
{
	friend class Game;
public:
	class SceneManagerException : public CornDescriptionException
	{
	public:
		SceneManagerException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static void CallLoadNextScene();
	static void CallLoadScene(UINT index);
	static void CallLoadPreviousScene();

	static SScene& GetConstScene();

	static SScene& GetCurrentScene();
	static int GetCurrentSceneIndex();
	static int GetNumberOfScene();
protected:
	static PSceneManager GetInstance();
	void StartLoadScene(UINT index);

	SceneManager();
	void Update();
	void Load();
	void Init();
protected:
	int sceneIndex;
	int callLoadSceneIndex;

	std::vector<SScene> scenes;
	SScene constScene;
	SScene currentScene;

	static PSceneManager __instance;
};

#define SCENEMANAGER_EXCEPT(description) SceneManager::SceneManagerException(__LINE__,__FILE__,description)