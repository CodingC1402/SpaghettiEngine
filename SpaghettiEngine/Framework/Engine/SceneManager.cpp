#include "SceneManager.h"
#include "SpaghettiEnginePath.h"
#include "json.hpp"
#include <fstream>

PSceneManager SceneManager::__instance = nullptr;

SceneManager::SceneManagerException::SceneManagerException(int line, const char* file, std::wstring discription) noexcept
	:
	CornDiscriptionException(line, file, discription)
{}

const wchar_t* SceneManager::SceneManagerException::GetType() const noexcept
{
	return L"Scene manager exception |ʘ‿ʘ)╯";
}

void SceneManager::Update()
{
	__instance->currentScene->Update();
	__instance->constScene->Update();
}

PSceneManager SceneManager::GetInstance()
{
	if (!__instance)
		__instance = new SceneManager();
	return __instance;
}

void SceneManager::LoadNextScene()
{
	LoadScene(__instance->sceneIndex + 1);
}

void SceneManager::LoadScene(int index)
{
	if (index < 0 || index > __instance->scenes.size())
		return;

	__instance->scenes[__instance->sceneIndex]->Unload();
	__instance->sceneIndex = index;
	__instance->currentScene = __instance->scenes[index];
	__instance->currentScene->Load();
}

SScene& SceneManager::GetCurrentScene()
{
	return __instance->currentScene;
}

int SceneManager::GetCurrentSceneIndex()
{
	return __instance->sceneIndex;
}

int SceneManager::GetNumberOfScene()
{
	return __instance->scenes.size();
}

void SceneManager::LoadPreviousScene()
{
	LoadScene(__instance->sceneIndex - 1);
}

SScene& SceneManager::GetConstScene()
{
	return __instance->constScene;
}

SceneManager::SceneManager()
	:
	sceneIndex(-1)
{}

#define CONSTSCENE "ConstScene"
#define SIZE "Size"
#define START "Start"

void SceneManager::Load()
{
	using namespace nlohmann;

	std::ifstream jsonStream(SCENEMANAGERPATH);
	if (!jsonStream.is_open())
	{
		std::wostringstream oss;
		oss << L"File ";
		oss << SCENEMANAGERPATH;
		oss << L" doesn't exist";
		throw SCENEMANAGER_EXCEPT(oss.str().c_str());
	}
	json file;
	try
	{
		jsonStream >> file;
		int size = file[SIZE].get<int>();
		for (int i = 0; i < size; i++)
		{
			scenes.push_back(SScene(new Scene(file[std::to_string(i)].get<std::string>())));
		}
		sceneIndex = file[START].get<int>();
		constScene = SScene(new Scene(file[CONSTSCENE].get<std::string>()));
	}
	catch (...)
	{
		std::wostringstream oss;
		oss << L"File ";
		oss << INPUTPATH;
		oss << L" is in the wrong format";
		throw SCENEMANAGER_EXCEPT(oss.str());
	}
}

void SceneManager::Init()
{
	Load();
	scenes[sceneIndex]->Load();
	constScene->Load();
}
