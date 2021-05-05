#include "Scene.h"
#include "CornException.h"
#include "GameObj.h"
#include "ScriptBase.h"
#include "Prefabs.h"
#include "Setting.h"
#include "LoadingJson.h"
#include <sstream>
#include <fstream>
#include <ranges>
#include <stack>

typedef std::shared_ptr<ScriptBase> SScriptBase;

Scene::Scene(std::string path)
	:
	path(std::move(path)),
	_tempComponentContainer(nullptr)
{}

Scene::SceneException::SceneException(int line, const char* file, std::string description)
	: CornException(line, file),
	_description(std::move(description))
{}

const wchar_t* Scene::SceneException::GetType() const noexcept
{
	return L"o(TヘTo) Scene exception";
}

const wchar_t* Scene::SceneException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString() << std::endl;
	os << "[Exception] "<< _description.c_str() << std::endl;
	
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

Scene::BaseComponent::BaseComponent(PScene owner, bool isDisabled)
	:
	_owner(owner),
	_isDisabled(isDisabled)
{}

void Scene::BaseComponent::Disable()
{
	if (_isDisabled)
		return;
	_isDisabled = true;
	OnDisabled();
}

void Scene::BaseComponent::Enable()
{
	if (!_isDisabled)
		return;
	_isDisabled = false;
	OnEnabled();
}

bool Scene::BaseComponent::IsDisabled()
{
	return _isDisabled;
}

void Scene::BaseComponent::Destroy()
{
	delete this;
}

void Scene::BaseComponent::DisableWithoutUpdate()
{
	_isDisabled = true;
}

void Scene::BaseComponent::EnableWithoutUpdate()
{
	_isDisabled = false;
}

std::shared_ptr<Scene::BaseComponent> Scene::BaseComponent::GetSharedPtr() const
{
	return _this.lock();
}

void Scene::BaseComponent::AssignSharedPtr(const std::shared_ptr<BaseComponent>& shared_ptr)
{
	_this = shared_ptr;
}

PScene Scene::BaseComponent::GetOwner() const
{
	return _owner;
}

void Scene::Start()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->OnStart();
}

void Scene::Update()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->OnUpdate();
}

void Scene::End()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->OnEnd();
}

void Scene::DestroyComponent(PBaseComponent component)
{
	component->Destroy();
}

void Scene::ConvertJsonAndAddComponent(SBaseComponent& component, nlohmann::json& json, ComponentType type) const
{
	using namespace  LoadingJson;
	
	if constexpr (Setting::IsDebugMode())
	{
		if (!ID::CheckID(json[idField].get<CULL>()))
			throw SCENE_EXCEPTION("[Exception] gameObj have local id lager then the number in setting file");
	}
	
	ID::ConvertIDInJson(json, type);
	
	_tempComponentContainer->emplace(json[idField].get<CULL>(), Entry(json[inputsField], component));
	if (json[isDisabled] != nullptr && json[isDisabled].get<bool>())
		component->DisableWithoutUpdate();
}
void Scene::Load()
{
	using namespace LoadingJson;
	
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "Scene file ";
		os << path.c_str();
		os << " Doesn't exist";
		throw SCENE_EXCEPTION(os.str());
	}
	
	try
	{
		_tempComponentContainer = new std::map<CULL, Entry>();
		
		json jsonFile;
		file >> jsonFile;

		if (jsonFile[gameObjectsField] == nullptr)
			throw std::exception();

		if (jsonFile[prefabsField] != nullptr)
		{
			std::shared_ptr<Prefab> currentPrefab;
			ULL requestedID = 0;
			for (int index = 1; auto& prefab : jsonFile[prefabsField])
			{
				CULL& newID = prefab[idField].get<CULL>();
				
				if constexpr (Setting::IsDebugMode())
				{
					if (newID == 0)
					{
						std::ostringstream os;
						os << "[Exception] ID can't be 0\n";
						os << "[Error file] " << "Scene file " << path.c_str() << std::endl;
						throw SCENE_EXCEPTION(os.str());
					}
				}
				
				if (requestedID != newID)
				{
					currentPrefab = PrefabsContainer::GetInstance()->GetResource(newID);
					requestedID = newID;
				}

				currentPrefab->Append(jsonFile, index, prefab[changesField]);
				index++;
			}
		}

		//Load script
		std::stack<PScriptBase> callOnEnableLater; // To only call enable for script cause obj enable is useless.
		for (auto& script : jsonFile[scriptsField])
		{
			callOnEnableLater.push(ScriptFactory::CreateInstance(script[inputsField][scriptTypeField].get<std::string>(), this));
			SBaseComponent newScript(callOnEnableLater.top(), DestroyComponent);
			newScript->AssignSharedPtr(newScript);
			ConvertJsonAndAddComponent(newScript, script, ComponentType::script);
		}
		//Load object
		for (auto& gameObj : jsonFile[gameObjectsField])
		{
			SBaseComponent newObj(new GameObj(this), DestroyComponent);
			newObj->AssignSharedPtr(newObj);
			ConvertJsonAndAddComponent(newObj, gameObj, ComponentType::gameObj);
		}
		//Call Load on all component
		for (auto& val : *_tempComponentContainer | std::views::values)
			val.Load();
		//Call enable on scripts
		while (!callOnEnableLater.empty())
		{
			if (!callOnEnableLater.top()->IsDisabled())
				callOnEnableLater.top()->OnEnabled();
			callOnEnableLater.pop();
		}
		
		_tempComponentContainer->clear();
		delete _tempComponentContainer;
		_tempComponentContainer = nullptr;
		file.close();

		PrefabsContainer::GetInstance()->UnloadResources();
	}
	catch (const CornException&)
	{
		throw;
	}
	catch (const std::exception& e)
	{
		delete _tempComponentContainer;
		_tempComponentContainer = nullptr;

		std::ostringstream os;
		os << "[Exception] An error occurs when trying to load scene " << path << std::endl;
		os << "[Std exception] " << e.what() << std::endl;
		
		throw SCENE_EXCEPTION(os.str());
	}
}

void Scene::Unload()
{
	for (PGameObj objPtr; auto& obj : _rootGameObjects)
	{
		objPtr = dynamic_cast<PGameObj>(obj.get());
		objPtr->RecursiveClearScript();
		objPtr->RecursiveMarkForDelete();
	}
}

Scene::Entry::Entry(json& loadJson, SBaseComponent& component)
	:
	_component(component),
	_loadJson(loadJson)
{}

void Scene::Entry::Load()
{
	_component->Load(_loadJson);
}

SGameObj Scene::CreateGameObject()
{
	SGameObj newObj = std::make_shared<GameObj>(this);
	newObj->AssignSharedPtr(newObj);
	return newObj;
}

SScriptBase Scene::CreateSpriteBase(const std::string& scriptName)
{
	SScriptBase newScript(ScriptFactory::CreateInstance(scriptName, this));
	newScript->AssignSharedPtr(newScript);
	return newScript;
}

Scene::SBaseComponent& Scene::GetComponent(CULL& id) const
{
	if (!_tempComponentContainer)
		throw SCENE_EXCEPTION("Trying to get component using id after load");
	auto tempPointer = _tempComponentContainer->find(id);
	if (tempPointer == _tempComponentContainer->end())
		throw SCENE_EXCEPTION("Cannot find the requested id");
	return tempPointer->second._component;
}

void Scene::PromoteGameObjToRoot(PGameObj gameObj)
{
	if constexpr (Setting::IsDebugMode())
	{
		for (auto& rootObj : _rootGameObjects)
			if (rootObj == gameObj->GetSharedPtr())
				throw CORN_EXCEPT_WITH_DESCRIPTION(L"[Exception] Trying to add root gameObj to root, please make sure that you check it in remove parent in game obj");
	}
	_rootGameObjects.remove(gameObj->GetSharedPtr());
	_rootGameObjects.push_back(gameObj->GetSharedPtr());
}

void Scene::DemoteGameObjFromRoot(PGameObj gameObj)
{
	_rootGameObjects.remove(gameObj->GetSharedPtr());
}