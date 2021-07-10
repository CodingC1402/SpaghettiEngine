#pragma once

#include "CornException.h"
#include "SMath.h"
#include "CollideEvent.h"
#include "Macros.h"
#include "RootContainer.h"

#include <string>
#include <list>
#include <map>
#include <memory>
#include <json.hpp>
#include <stack>

CLASS_FORWARD_DECLARATION(GameObj);
CLASS_FORWARD_DECLARATION(ScriptBase);
CLASS_FORWARD_DECLARATION(Scene);
CLASS_FORWARD_DECLARATION(BaseComponent);

using namespace nlohmann;
class Scene
{
    friend class BaseComponent;
    friend class GameObj;
    friend class SceneManager;
    friend class ChildContainer;
public:
    class SceneException : public CornException
    {
    public:
        SceneException(int line, const char* file, std::string description);
        const wchar_t* GetType() const noexcept override;
        const wchar_t* What() const noexcept override;
    protected:
        std::string _description;
    };

    class Entry
    {
    public:
        Entry(json& loadJson, SBaseComponent& component);
        void Load();
    public:
        SBaseComponent _component;
        json _loadJson;
    };
public:
    // Create game object and tell smart pointer what to use
    [[nodiscard]] PGameObj CreateGameObject(bool isDisabled);
    // Create script and tell smart pointer what to use
    [[nodiscard]] PScriptBase CreateScriptBase(const std::string& scriptName, bool isDisabled);

    [[nodiscard]] PBaseComponent GetComponent(CULL& id) const;

    PGameObj Instantiate(GameObj* toClone, Vector3 worldPosition);

    // The actual function to tell smart pointer to destroy component with Destroy function.
    static void DestroyComponent(PBaseComponent component);
protected:
    Scene(std::string path);

    void AddToTrashBin(SBaseComponent destroyedComponent);
    void EraseTrashBin();

    void AddToRoot(const PGameObj& object);
    void RemoveFromRoot(const PGameObj& object);

    void Start();
    void Update();
    void FixedUpdate();

    void SetUpAddComponent(SBaseComponent& component, nlohmann::json& json);

    // Compile script to create needed gameObject and scripts
    void Load();
    // Actually call the gameObj and script to load
    void LoadComponent();
    // Well unload stuff.
    void Unload();
protected:
    std::string path;

    // Call each time before game object update
    std::list<SBaseComponent>   _trashBin;
    std::list<SBaseComponent>   _gameObjects;
    RootContainer               _rootContainer;
    std::list<SBaseComponent>   _scripts;

    std::map<CULL, Entry>* _tempComponentContainer = nullptr;
    std::stack<PScriptBase>* _callEnable = nullptr;
};

#define SCENE_EXCEPTION(description) Scene::SceneException(__LINE__,__FILE__,description)
