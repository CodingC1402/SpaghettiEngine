#include "Scene.h"
#include "CornException.h"
#include "GameObj.h"
#include "ScriptBase.h"
#include "Prefabs.h"
#include "Setting.h"
#include "LoadingJson.h"
#include "App.h"
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

Scene::BaseComponent::BaseComponent(PScene owner, bool isDisabled)
    :
    _owner(owner),
    _isDisabled(isDisabled)
{}

void Scene::BaseComponent::Disable()
{
    if (_isDisabled)
        return;
    OnDisabled();
    _isDisabled = true;
}

void Scene::BaseComponent::Enable()
{
    if (!_isDisabled)
        return;
    OnEnabled();
    _isDisabled = false;
}

bool Scene::BaseComponent::IsDisabled()
{
    return _isDisabled;
}

void Scene::BaseComponent::Destroy()
{
    if (!_isDisabled)
        this->Disable();
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

void Scene::Enable()
{
    if (_callEnable)
    {
        while (!_callEnable->empty())
        {
            _callEnable->top()->OnEnabled();
            _callEnable->pop();
        }
        delete _callEnable;
        _callEnable = nullptr;
    }
    else
    {
        throw SCENE_EXCEPTION("Call enable more than once or call enable before load");
    }
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

void Scene::FixedUpdate()
{
    for (const auto& gameObj : _rootGameObjects)
        gameObj->OnFixedUpdate();
}

void Scene::End()
{
    for (const auto& gameObj : _rootGameObjects)
        gameObj->OnEnd();
}

SGameObj Scene::Instantiate(GameObj* toClone, Vector3 worldPosition)
{
    auto clonedGameObj = std::dynamic_pointer_cast<GameObj>(toClone->Clone());
    clonedGameObj->Move(worldPosition);
    return clonedGameObj;
}

void Scene::DestroyComponent(PBaseComponent component)
{
    component->Destroy();
}

void Scene::SetUpAddComponent(SBaseComponent& component, nlohmann::json& json, ComponentType type)
{
    using LoadingJson::Field;
    component->AssignSharedPtr(component);
    _tempComponentContainer->emplace(json[Field::idField].get<CULL>(), Entry(json[Field::inputsField], component));
    if (!json[Field::isDisabled].empty() && json[Field::isDisabled].get<bool>())
        component->DisableWithoutUpdate();
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
            _callEnable->push(ScriptFactory::CreateInstance(script[Field::inputsField][Field::scriptTypeField].get<std::string>(), this));
            SBaseComponent newScript(_callEnable->top(), DestroyComponent);
            SetUpAddComponent(newScript, script, ComponentType::script);
        }
        //Load object
        for (auto& gameObj : jsonFile[Field::gameObjectsField])
        {
            SBaseComponent newObj(new GameObj(this), DestroyComponent);
            SetUpAddComponent(newObj, gameObj, ComponentType::gameObj);
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
    auto app = App::GetInstance();
    for (PGameObj objPtr; auto & obj : _rootGameObjects)
    {
        objPtr = dynamic_cast<PGameObj>(obj.get());
        objPtr->RecursiveClearScriptsWithoutCallEnd();
        objPtr->RecursiveMarkForDelete();
    }
    _rootGameObjects.clear(); 
}

void Scene::Disable()
{
    for (PGameObj objPtr; auto & obj : _rootGameObjects)
    {
        objPtr = dynamic_cast<PGameObj>(obj.get());
        objPtr->Disable();
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
    {
        std::ostringstream os;
        os << "Could not find the requested id";
        os << "[ID] " << id << std::endl;
        throw SCENE_EXCEPTION(os.str());
    }
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