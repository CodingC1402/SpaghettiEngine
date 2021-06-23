#include "Scene.h"
#include "CornException.h"
#include "GameObj.h"
#include "ScriptBase.h"
#include "Prefabs.h"
#include "Setting.h"
#include "LoadingJson.h"
#include "App.h"
#include "BaseComponent.h"

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
    os << "[Exception] " << _description.c_str() << std::endl;

    whatBuffer = os.str();
    return whatBuffer.c_str();
}

void Scene::Start()
{
    for (const auto& gameObj : _gameObjects)
        gameObj->OnStart();

    for (const auto& gameObj : _gameObjects)
        if (!gameObj->IsDisabled())
            gameObj->OnEnabled();
}

void Scene::Update()
{
    for (auto it = _gameObjects.begin(); it != _gameObjects.end();)
    {
        (*it)->OnUpdate();
        ++it;
    }

    EraseTrashBin();
}

void Scene::FixedUpdate()
{
    for (const auto& gameObj : _gameObjects)
        gameObj->OnFixedUpdate();
}

void Scene::End()
{
    for (const auto& gameObj : _gameObjects)
        gameObj->OnEnd();
}

PGameObj Scene::Instantiate(GameObj* toClone, Vector3 worldPosition)
{
    auto clonedGameObj = toClone->Clone();

    AddToRoot(clonedGameObj);
    clonedGameObj->OnStart();
    clonedGameObj->OnEnabled();
    clonedGameObj->GetTransform().Move(worldPosition);

    return clonedGameObj;
}

void Scene::DestroyComponent(PBaseComponent component)
{
    component->Destroy();
}

void Scene::SetUpAddComponent(SBaseComponent& component, nlohmann::json& json)
{
    using LoadingJson::Field;
    _tempComponentContainer->emplace(json[Field::idField].get<CULL>(), Entry(json[Field::inputsField], component));
    if (!json[Field::nameField].empty())
        component->SetName(json[Field::nameField].get<std::string>());
    if (!json[Field::isDisabled].empty() && json[Field::isDisabled].get<bool>())
        component->_isDisabled = true;
}
void Scene::Load()
{
    try
    {
        using LoadingJson::Field;

        Prefab sceneJson;
        json jsonFile;
        json changes;

        unsigned index = 0;
        sceneJson.Load(path);
        SPrefabHierarchy prefabHierarchy = sceneJson.Append(jsonFile, index, changes);
        LoadingJson::ID::ConvertIDInJson(jsonFile, prefabHierarchy);

        _tempComponentContainer = new std::map<CULL, Entry>();
        if (!_callEnable)
            _callEnable = new std::stack<PScriptBase>();
        //Load script
        for (auto& script : jsonFile[Field::scriptsField])
        {
            // No need to call enable for gameObj cause it's useless and save it for in game enable/disable
            auto scriptType = script[Field::inputsField][Field::scriptTypeField].get<std::string>();
            SBaseComponent newScript = CreateScriptBase(scriptType, false)->GetSharedPtr();
            SetUpAddComponent(newScript, script);
        }
        //Load object
        for (auto& gameObj : jsonFile[Field::gameObjectsField])
        {
            SBaseComponent newObj = CreateGameObject(false)->GetSharedPtr();
            SetUpAddComponent(newObj, gameObj);
        }
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

void Scene::LoadComponent()
{
    for (auto& val : *_tempComponentContainer | std::views::values)
        val.Load();

    _tempComponentContainer->clear();
    delete _tempComponentContainer;
    _tempComponentContainer = nullptr;
}

void Scene::Unload()
{
    _scripts.clear();
    _gameObjects.clear();
    _trashBin.clear();
}

void Scene::AddToTrashBin(SBaseComponent destroyedComponent)
{
    _trashBin.push_back(destroyedComponent);
}

void Scene::EraseTrashBin()
{
    for (const auto& component : _trashBin)
    {
        switch (component->GetType())
        {
        case BaseComponent::Type::gameObj:
            _gameObjects.erase(component->GetIterator());
            break;
        case BaseComponent::Type::script:
            _scripts.erase(component->GetIterator());
            break;
        default:
            break;
        }
    }
    _trashBin.clear();
}

void Scene::AddToRoot(const PGameObj& object)
{
    if (!object)
        return;

    _rootObjects.push_back(object);
    object->SetContainerIterator(--_rootObjects.end());
}

void Scene::RemoveFromRoot(const PGameObj& object)
{
    if (!object)
        return;

    _rootObjects.erase(object->GetContainerIterator());
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

PGameObj Scene::CreateGameObject(bool isDisabled)
{
    SGameObj newObj = std::make_shared<GameObj>(this, isDisabled, DestroyComponent);

    _gameObjects.push_back(newObj);
    auto it = --_gameObjects.end();
    newObj->AssignPtr(it);

    return newObj.get();
}

PScriptBase Scene::CreateScriptBase(const std::string& scriptName, bool isDisabled)
{
    SScriptBase newScript(ScriptFactory::CreateInstance(scriptName, this), DestroyComponent);

    _scripts.push_back(newScript);
    auto it = --_scripts.end();
    newScript->AssignPtr(it);

    return newScript.get();
}

SBaseComponent& Scene::GetComponent(CULL& id) const
{
    if (!_tempComponentContainer)
        throw SCENE_EXCEPTION("Trying to get component using id after load");
    auto tempPointer = _tempComponentContainer->find(id);
    if (tempPointer == _tempComponentContainer->end())
    {
        std::ostringstream os;
        os << "Could not find the requested id";
        os << "[ID] " << id << std::endl;
        throw SCENE_EXCEPTION(os.str());
    }
    return tempPointer->second._component;
}