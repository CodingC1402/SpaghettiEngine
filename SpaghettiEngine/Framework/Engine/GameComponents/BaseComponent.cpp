#include "BaseComponent.h"

BaseComponent::BaseComponent(PScene owner, bool isDisabled)
{
    _owner = owner;
    _isDisabled = isDisabled;
}

void BaseComponent::Disable()
{
    if (_isDisabled)
        return;
    OnDisabled();
    _isDisabled = true;
}

void BaseComponent::Enable()
{
    if (!_isDisabled)
        return;
    OnEnabled();
    _isDisabled = false;
}

bool BaseComponent::IsDisabled() const
{
    return _isDisabled;
}

bool BaseComponent::CallDestroy()
{
    if (_owner)
    {
        _owner->AddToTrashBin(_this.lock());
        return true;
    }
    return false;
}

void BaseComponent::Destroy()
{
    if (!_isDisabled)
        this->Disable();
    // Call on end when the object is destroyed
    this->OnEnd();

    delete this;
}

void BaseComponent::SetName(const std::string& name)
{
    _name = name;
}

std::string BaseComponent::GetName()
{
    return _name;
}

std::list<SBaseComponent>::iterator BaseComponent::GetIterator() const
{
    return _iterator;
}

std::shared_ptr<BaseComponent> BaseComponent::GetSharedPtr() const
{
    return _this.lock();
}

void BaseComponent::AssignPtr(std::list<SBaseComponent>::iterator iterator)
{
    _iterator = iterator;
    _this = *iterator;
}

PScene BaseComponent::GetOwner() const
{
    return _owner;
}

void BaseComponent::SetOwner(PScene owner)
{
    _owner = owner;
}

