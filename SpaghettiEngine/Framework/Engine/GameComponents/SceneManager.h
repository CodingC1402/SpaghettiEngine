#pragma once
#include "CornException.h"
#include "Scene.h"
#include <vector>
#include <mutex>

typedef class SceneManager* PSceneManager;
typedef class GameObj* PGameObj;

class SceneManager
{
	friend class Game;
public:
	class SceneManagerException : public CornException
	{
	public:
		SceneManagerException(int line, const char* file, std::string description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		const wchar_t* What() const noexcept override;
	protected:
		std::string _description;
	};
public:
	static void CallLoadNextScene();
	static void CallLoadScene(unsigned index);
	static void CallLoadPreviousScene();

	static SScene& GetConstScene();

	static SScene& GetCurrentScene();
	static int GetCurrentSceneIndex();
	static int GetNumberOfScene();
protected:
	static PSceneManager GetInstance();
	void StartLoadScene(SScene current, SScene toLoad);
	
	void CleanUpAfterLoad();

	SceneManager();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void CleanUp();

	void Load();
	void Init();
	void Unload();
protected:
	//std::atomic<unsigned> sceneIndex;
	//std::atomic<unsigned> callLoadSceneIndex;

	unsigned sceneIndex;
	unsigned callLoadSceneIndex;

	std::atomic_bool _isLoading = false;
	bool _startedLoadNewScene = false;

	std::recursive_mutex _sceneLock;

	std::vector<SScene> scenes;
	SScene constScene;

	static PSceneManager __instance;
};

#define SCENEMANAGER_EXCEPT(description) SceneManager::SceneManagerException(__LINE__,__FILE__,description)