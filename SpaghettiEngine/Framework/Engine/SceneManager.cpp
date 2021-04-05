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

	__instance->scenes[__instance->sceneIndex].Unload();
	__instance->sceneIndex = index;
	__instance->scenes[index].Load();
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

SceneManager::SceneManager()
	:
	sceneIndex(-1)
{}

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
		int size = file["Size"].get<int>();
		for (int i = 0; i < size; i++)
		{
			scenes.push_back(file[std::to_string(i)].get<std::string>());
		}
		int start = file["Start"].get<int>();
		LoadScene(start);
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

}
