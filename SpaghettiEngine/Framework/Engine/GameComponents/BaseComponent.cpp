#include "BaseComponent.h"

BaseComponent::BaseComponent(PScene owner, bool isDisabled)
{
    _owner = owner;
    _isDisabled = isDisabled;
}


//============================================={NOTE}===============================================//
// These two are not mistake, base on the decision table:
// 
// C\P    | Enable | Disable | Result
// -------+--------+---------+-----------
// Enable |   X    |         | OnEnabled
// Disable|   X    |         | Nothing
// Enable |        |    X    | OnDisabled
// Disable|        |    X    | Nothing
//
// You can see that only when an object is enabled it would care about what the parent do,
// that make sense but will cause error because the OnEnabled doesn't know whether it is
// it own event or other so it check by using.
// if (IsDisabled())
// So to make thing work we have to set isDisabled first on Enable and after in Disable.
// Got that future me?

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
    _isDisabled = false;
    OnEnabled();
}
//============================================={NOTE}===============================================//

bool BaseComponent::IsDeleted() const
{
    return _isDeleted;
}

bool BaseComponent::IsDisabled() const
{
    return _isDisabled;
}

bool BaseComponent::CallDestroy()
{
    if (_owner && !IsDeleted())
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

std::string BaseComponent::GetName() const
{
    return _name;
}

bool BaseComponent::SetToDeleted()
{
    if (IsDeleted())
        return false;
    _isDeleted = true;
    return true;
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

