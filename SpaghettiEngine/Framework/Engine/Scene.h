#pragma once

#include "CornException.h"
#include "ExMath.h"
#include <string>
#include <list>
#include <map>
#include <memory>
#include <json.hpp>

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;

typedef class ScriptBase* PScriptBase;

typedef class Scene* PScene;
typedef std::unique_ptr<Scene> UScene;
typedef std::shared_ptr<Scene> SScene;

using namespace nlohmann;
class Scene
{
    friend class GameObj;
    friend class SceneManager;
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

    enum class ComponentType
    {
        invalid = -1,
        gameObj = 1,
        script = 2
    };
    typedef class BaseComponent
    {
    public:
        BaseComponent(PScene owner, bool isDisabled = false);

        BaseComponent(const BaseComponent&) = delete;
        BaseComponent(const BaseComponent&&) = delete;
        BaseComponent& operator=(const BaseComponent&) = delete;
        BaseComponent& operator=(const BaseComponent&&) = delete;

        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnEnd() {}

        virtual void OnDisabled() {}
        virtual void OnEnabled() {}

        virtual void Disable();
        virtual void Enable();

        virtual bool [[nodiscard]] IsDisabled();

        virtual void Load(nlohmann::json& input) = 0;
        virtual void Destroy();

        //Don't use
        void DisableWithoutUpdate();
        //Don't use
        void EnableWithoutUpdate();

        virtual std::shared_ptr<BaseComponent> Clone() = 0;

        /// Get the shared_ptr of the component which is owned by a scene
        [[nodiscard]] std::shared_ptr<BaseComponent> GetSharedPtr() const;
        void AssignSharedPtr(const std::shared_ptr<BaseComponent>& shared_ptr);
        [[nodiscard]] PScene GetOwner() const;
    protected:
        virtual ~BaseComponent() = default;
    protected:
        PScene _owner = nullptr;
        bool _isDisabled;
        std::weak_ptr<BaseComponent> _this;
    } *PBaseComponent;
    typedef std::shared_ptr<BaseComponent> SBaseComponent;
    typedef std::weak_ptr<BaseComponent> WBaseComponent;

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
    // Only work in load
    [[nodiscard]] SGameObj CreateGameObject();
    [[nodiscard]] std::shared_ptr<ScriptBase> CreateSpriteBase(const std::string& scriptName);
    [[nodiscard]] SBaseComponent& GetComponent(CULL& id) const;

    void Start();
    void Update();
    void End();

    static void DestroyComponent(PBaseComponent component);
protected:
    Scene(std::string path);

    void PromoteGameObjToRoot(PGameObj gameObj);
    void DemoteGameObjFromRoot(PGameObj gameObj);

    void SetUpAddComponent(SBaseComponent& component, nlohmann::json& json, ComponentType type) const;
    void Load();
    void Unload();
protected:
    std::string path;

    // Will update every loop
    std::list<SBaseComponent> _rootGameObjects;
    std::map<CULL, Entry>* _tempComponentContainer;
};

#define SCENE_EXCEPTION(description) Scene::SceneException(__LINE__,__FILE__,description)
