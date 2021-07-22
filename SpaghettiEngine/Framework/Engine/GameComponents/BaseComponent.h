#pragma once
#include "Scene.h"
#include <list>

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

    void SetName(const std::string& name);
    [[nodiscard]] std::string GetName();

#pragma region Events
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnFixedUpdate() {}
    virtual void OnLateUpdate() {}
    virtual void OnEnd() {}

    virtual void OnDisabled() {}
    virtual void OnEnabled() {}

    virtual void Disable();
    virtual void Enable();

    virtual void OnCollide(CollideEvent& e) {};
#pragma endregion

    [[nodiscard]] bool IsDeleted() const;

    [[nodiscard]] virtual bool IsDisabled() const;
    virtual void Load(nlohmann::json& input) = 0;

    virtual bool CallDestroy();
    virtual Type GetComponentType() const = 0;

    /// Get the shared_ptr of the component which is owned by a scene
    [[nodiscard]] std::shared_ptr<BaseComponent> GetSharedPtr() const;
    void AssignPtr(std::list<SBaseComponent>::iterator iterator);

    [[nodiscard]] PScene GetOwner() const;
    void SetOwner(PScene owner);
protected:

    virtual void Destroy();
    virtual ~BaseComponent() = default;
private: // Can only be used by scene
    bool SetToDeleted();
    std::list<SBaseComponent>::iterator GetIterator() const;
private:
    PScene _owner = nullptr;
    bool _isDisabled = false;
    bool _isDeleted = false;

    std::string _name;

    // the pointer to it self in the scene container.
    std::list<SBaseComponent>::iterator _iterator;
    std::weak_ptr<BaseComponent> _this;
};