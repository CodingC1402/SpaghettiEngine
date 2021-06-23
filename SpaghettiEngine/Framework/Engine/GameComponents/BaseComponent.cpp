#include "BaseComponent.h"

BaseComponent::BaseComponent(PScene owner, bool isDisabled)
    :
    _owner(owner),
    _isDisabled(isDisabled)
{}

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

bool BaseComponent::IsDisabled()
{
    return _isDisabled;
}

void BaseComponent::Destroy()
{
    if (!_isDisabled)
        this->Disable();
    delete this;
}

void BaseComponent::DisableWithoutUpdate()
{
    _isDisabled = true;
}

void BaseComponent::EnableWithoutUpdate()
{
    _isDisabled = false;
}

std::shared_ptr<BaseComponent> BaseComponent::GetSharedPtr() const
{
    return _this.lock();
}

void BaseComponent::AssignSharedPtr(const std::shared_ptr<BaseComponent>& shared_ptr)
{
    _this = shared_ptr;
}

PScene BaseComponent::GetOwner() const
{
    return _owner;
}

