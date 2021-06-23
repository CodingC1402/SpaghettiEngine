#pragma once
#include "Scene.h"

CLASS_FORWARD_DECLARATION(BaseComponent);

class BaseComponent
{
    friend class Scene;
public:
    // Used to convert local id to public id.
    enum class Type
    {
        gameObj = 1,
        script = 2,
        invalid = 3
    };
public:
    BaseComponent(PScene owner, bool isDisabled = false);

    BaseComponent(const BaseComponent&) = delete;
    BaseComponent(const BaseComponent&&) = delete;
    BaseComponent& operator=(const BaseComponent&) = delete;
    BaseComponent& operator=(const BaseComponent&&) = delete;

#pragma region Events
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnFixedUpdate() {}
    virtual void OnEnd() {}

    virtual void OnDisabled() {}
    virtual void OnEnabled() {}

    virtual void Disable();
    virtual void Enable();

    virtual void OnCollide(CollideEvent& e) {};
    virtual void OnCollideEnter(CollideEvent& e) {};
    virtual void OnCollideExit(CollideEvent& e) {};
#pragma endregion

    virtual bool [[nodiscard]] IsDisabled();
    virtual void Load(nlohmann::json& input) = 0;
    virtual void Destroy();

    /// Get the shared_ptr of the component which is owned by a scene
    [[nodiscard]] std::shared_ptr<BaseComponent> GetSharedPtr() const;
    void AssignSharedPtr(const std::shared_ptr<BaseComponent>& shared_ptr);
    [[nodiscard]] PScene GetOwner() const;
protected:
    virtual ~BaseComponent() = default;
private:    
    //Don't use
    void DisableWithoutUpdate();
    //Don't use
    void EnableWithoutUpdate();
protected:
    PScene _owner = nullptr;
    bool _isDisabled = false;
    std::weak_ptr<BaseComponent> _this;
};