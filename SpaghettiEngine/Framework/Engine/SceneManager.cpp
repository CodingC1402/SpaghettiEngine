#include "SceneManager.h"
#include "SpaghettiEnginePath.h"
#include "json.hpp"
#include "CornException.h"
#include "Animation.h"
#include "Graphics.h"
#include "Physic.h"
#include <fstream>

PSceneManager SceneManager::__instance = nullptr;

SceneManager::SceneManagerException::SceneManagerException(int line, const char* file, std::wstring description) noexcept
	:
	CornDescriptionException(line, file, description)
{}

const wchar_t* SceneManager::SceneManagerException::GetType() const noexcept
{
	return L"Scene manager exception |ʘ‿ʘ)╯";
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

	if (index > scenes.size())
	{
		std::wostringstream os;
		os << L"LoadScene called with index ";
		os << index << std::endl;
		os << L"But the number of scenes is ";
		os << scenes.size() << std::endl;
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}

	Physic::GetInstance()->Unload();
	Graphics::GetInstance()->ClearRenderBuffer();
	currentScene = scenes[index];
	currentScene->Load();
	scenes[sceneIndex]->Unload();
	sceneIndex = index;

	Animation::ClearUnusedAnimation();
	Texture::ClearUnusedTexture();
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

#define CONSTSCENE "ConstScene"
#define SCENES "Scenes"
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
		for (const auto& scene : file[SCENES])
			scenes.push_back(SScene(new Scene(scene.get<std::string>())));

		sceneIndex = file[START].get<int>();
		callLoadSceneIndex = sceneIndex;
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
	currentScene = scenes[sceneIndex];
	constScene->Load();
}
