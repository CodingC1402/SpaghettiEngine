#include "SceneManager.h"
#include "SpaghettiEnginePath.h"
#include "json.hpp"
#include "CornException.h"
#include "Animation.h"
#include "Graphics.h"
#include "Path.h"
#include <fstream>

PSceneManager SceneManager::__instance = nullptr;

SceneManager::SceneManagerException::SceneManagerException(int line, const char* file, std::string description) noexcept
	:
	CornException(line, file),
	_description(description)
{}

const wchar_t* SceneManager::SceneManagerException::GetType() const noexcept
{
	return L"Scene manager exception |ʘ‿ʘ)╯";
}

const wchar_t* SceneManager::SceneManagerException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString() << std::endl;
	os << _description.c_str();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

void SceneManager::Update()
{
	if (callLoadSceneIndex != sceneIndex)
		StartLoadScene(callLoadSceneIndex);
	__instance->currentScene->Update();
	__instance->constScene->Update();
}

PSceneManager SceneManager::GetInstance()
{
	if (!__instance)
		__instance = new SceneManager();
	return __instance;
}

void SceneManager::StartLoadScene(UINT index)
{
	if (index == sceneIndex)
		return;

	if (index >= scenes.size())
	{
		std::ostringstream os;
		os << "LoadScene called with index ";
		os << index << std::endl;
		os << "But the number of scenes is ";
		os << scenes.size() << std::endl;
		throw SCENEMANAGER_EXCEPT(os.str());
	}

	currentScene->End();
	scenes[index]->Load();
	currentScene->Unload();
	scenes[index]->Start();
	
	sceneIndex = index;
	currentScene = scenes[sceneIndex];
	
	AnimationContainer::GetInstance()->UnloadUnusedResources();
	TextureContainer::GetInstance()->UnloadUnusedResources();
}

void SceneManager::CallLoadNextScene()
{
	GetInstance()->callLoadSceneIndex = GetInstance()->sceneIndex + 1;
}

void SceneManager::CallLoadScene(UINT index)
{
	GetInstance()->callLoadSceneIndex = index;
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
	return static_cast<int>(__instance->scenes.size());
}

void SceneManager::CallLoadPreviousScene()
{
	GetInstance()->callLoadSceneIndex = GetInstance()->sceneIndex - 1;
}

SScene& SceneManager::GetConstScene()
{
	return __instance->constScene;
}

SceneManager::SceneManager()
	:
	sceneIndex(-1),
	callLoadSceneIndex(-1)
{}

void SceneManager::Load()
{
	using namespace nlohmann;

	std::ifstream jsonStream(SystemPath::SceneManagerPath);
	if (!jsonStream.is_open())
	{
		std::ostringstream oss;
		oss << "[Exception] File ";
		oss << SystemPath::SceneManagerPath;
		oss << " doesn't exist";
		throw SCENEMANAGER_EXCEPT(oss.str());
	}
	try
	{
		static constexpr const char* StaticScene = "StaticScene";
		static constexpr const char* Scenes = "Scenes";
		static constexpr const char* Start = "Start";
		
		json file;
		jsonStream >> file;
		for (const auto& scene : file[Scenes])
			scenes.push_back(SScene(new Scene(CLib::ConvertPath(SystemPath::SceneManagerPath, scene.get<std::string>()))));

		sceneIndex = file[Start].get<int>();
		callLoadSceneIndex = sceneIndex;
		constScene = SScene(new Scene(file[StaticScene].get<std::string>()));
	}
	catch (...)
	{
		std::ostringstream oss;
		oss << "[Exception] File ";
		oss << SystemPath::SceneManagerPath;
		oss << " is in the wrong format";
		throw SCENEMANAGER_EXCEPT(oss.str());
	}
}

void SceneManager::Init()
{
	Load();
	scenes[sceneIndex]->Load();
	currentScene = scenes[sceneIndex];
	constScene->Load();
	currentScene->Start();
	constScene->Start();
}
