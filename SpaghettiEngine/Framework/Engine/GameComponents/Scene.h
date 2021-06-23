#pragma once

#include "CornException.h"
#include "SMath.h"
#include "CollideEvent.h"
#include "Macros.h"

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
    [[nodiscard]] std::shared_ptr<ScriptBase> CreateScriptBase(const std::string& scriptName);
    [[nodiscard]] SBaseComponent& GetComponent(CULL& id) const;

    SGameObj Instantiate(GameObj* toClone, Vector3 worldPosition);
    static void DestroyComponent(PBaseComponent component);
protected:
    Scene(std::string path);

    void PromoteGameObjToRoot(PGameObj gameObj);
    void DemoteGameObjFromRoot(PGameObj gameObj);

    void Enable(); // Call after load from main thread
    void Disable(); // Call before unload from scene manager

    void Start();
    void Update();
    void FixedUpdate();
    void End();

    void SetUpAddComponent(SBaseComponent& component, nlohmann::json& json);
    void Load();
    void LoadComponent();
    void Unload();
protected:
    std::string path;

    // Will update every loop
    std::list<SGameObj> _gameObjects;
    std::list<SScriptBase> _scripts;
    std::map<CULL, Entry>* _tempComponentContainer = nullptr;
    std::stack<PScriptBase>* _callEnable = nullptr;
};

#define SCENE_EXCEPTION(description) Scene::SceneException(__LINE__,__FILE__,description)
