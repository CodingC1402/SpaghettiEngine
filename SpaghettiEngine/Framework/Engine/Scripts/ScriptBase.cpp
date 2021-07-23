#include "ScriptBase.h"

#pragma region ScriptTypes
#include "Animator.h"
#include "Camera.h"
#include "Canvas.h"
#include "CircleCollider.h"
#include "Polygon2DCollider.h"
#include "RigidBody2D.h"
#include "SoundSource.h"
#include "SpriteRenderer.h"
#include "Surface.h"
#include "TileMapRenderer.h"
#pragma endregion

//
// Theory: I think what happen is that the linker when linking the .h file and .cpp file
// they find the function below and think that we might need the class in the future so it
// added the class implementation the project that use this static lib.
// Another point is that when you use the static constructor macro, somehow including
// it is enough to make it register properly, might need future investigation in CLang
// compiler.
//
void REGISTERFUNCTION()
{
	Animator one(nullptr);
	Camera two(nullptr);
	Canvas three(nullptr);
	CircleCollider four(nullptr);
	Polygon2DCollider five(nullptr);
	RigidBody2D six(nullptr);
	SpriteRenderer seven(nullptr);
	Surface eight(nullptr);
	TileMapRenderer nine(nullptr);
	SoundSource ten(nullptr);
}
ScriptTypes* ScriptFactory::map = nullptr;

ScriptTypes* ScriptFactory::GetMap()
{
	if (!map)
		map = new ScriptTypes();
	return map;
}

PScriptBase ScriptFactory::CreateInstance(std::string const& typeName, PScene owner, bool isDisabled)
{
	const auto iterator = map->find(typeName);
	if (iterator == GetMap()->end())
		return nullptr;
	return static_cast<PScriptBase>(iterator->second(owner, isDisabled));
}

ScriptBase::ScriptException::ScriptException(int line, const char* file, PScriptBase errorScript, const std::wstring& extraDescription)
	: CornException(line, file),
	_errorScript(errorScript),
	_extraDescription(extraDescription)
{}

const wchar_t* ScriptBase::ScriptException::GetType() const noexcept
{
	return L"(╬ Ò﹏Ó) Script format error";
}

const wchar_t* ScriptBase::ScriptException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString().c_str() << std::endl;
	os << L"[Script type] " << _errorScript->GetType().c_str() << std::endl;
	os << L"[Error] " << L"Wrong format" << std::endl;
	os << L"[Extra description] " << _extraDescription.c_str();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

ScriptBase::ScriptBase(PScene owner, bool isDisabled)
	:
	BaseComponent(owner, isDisabled)
{}

void ScriptBase::SetGameObject(const PGameObj& owner)
{
	if (!owner && !_ownerObj)
		return;

	if (owner)
		owner->GetScriptContainer().AddItem(this);
	else
		_ownerObj->GetScriptContainer().RemoveItem(this);

	if (!(_ownerObj || this->IsDisabled()))
		this->Disable();
}

Matrix4 ScriptBase::GetWorldMatrix() const noexcept
{
	return _ownerObj->GetTransform().GetWorldMatrix();
}

Vector3 ScriptBase::GetWorldTransform() const noexcept
{
	return _ownerObj->GetTransform().GetWorldTransform();
}

Vector3 ScriptBase::GetWorldRotation() const noexcept
{
	return _ownerObj->GetTransform().GetWorldRotation();
}
Vector3 ScriptBase::GetWorldScale() const noexcept
{
	return _ownerObj->GetTransform().GetWorldScale();
}

PGameObj ScriptBase::GetGameObject() const noexcept
{
	return _ownerObj;
}

bool ScriptBase::IsDisabled() const noexcept
{
	return BaseComponent::IsDisabled() || (GetGameObject() && GetGameObject()->IsDisabled());
}

PScriptBase ScriptBase::Clone() const
{
	auto cloneScript = GetOwner()->CreateScriptBase(GetType(), BaseComponent::IsDisabled());

	cloneScript->SetName(GetName());

	return cloneScript;
}

void ScriptBase::Load(nlohmann::json& input)
{
}

BaseComponent::Type ScriptBase::GetComponentType() const
{
	return BaseComponent::Type::script;
}

void ScriptBase::Destroy()
{
	this->Disable();

	if (_ownerObj)
		_ownerObj->GetScriptContainer().RemoveItem(this);

	BaseComponent::Destroy();
}

void ScriptBase::SetContainerIterator(std::list<PScriptBase>::iterator it)
{
	_containerIterator = it;
}

std::list<PScriptBase>::iterator ScriptBase::GetContainerIterator() const
{
	return _containerIterator;
}
