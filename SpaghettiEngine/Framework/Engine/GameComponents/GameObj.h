#pragma once
#include "json.hpp"
#include "CornException.h"
#include "PhysicComponent.h"
#include "Scene.h"
#include "BaseComponent.h"
#include "ChildContainer.h"
#include "ScriptContainer.h"
#include "Transform.h"

#include <memory>
#include <list>
#include <string>

CLASS_FORWARD_DECLARATION(ScriptBase);
CLASS_FORWARD_DECLARATION(GameObj);

using std::string;
using std::list;
class GameObj : public BaseComponent
{
	friend class Physic;
	friend class Scene;
	friend class PhysicComponent;
	friend class ChildContainer;
	friend class RootContainer;
public:
	class GameObjectFormatException : public CornException
	{
	public:
		GameObjectFormatException(int line, const char* file, const char* errorField, PGameObj errorObj, const char* extraDescription)
			: CornException(line, file), _errorField(errorField), _errorObj(errorObj), _extraDescription(extraDescription) {}
		const wchar_t* GetType() const noexcept override
		{
			return L"(」°ロ°)」Game object format exception";
		}
		const wchar_t* What() const noexcept override
		{
			std::wostringstream os;
			os << GetOriginString() << std::endl;
			os << "[Error field] " << _errorField.c_str() << std::endl;
			os << "[Extra description] " << _extraDescription.c_str() << std::endl;
			whatBuffer = os.str();
			return whatBuffer.c_str();
		}
	protected:
		string _errorField;
		PGameObj _errorObj;
		string _extraDescription;
	};
public:
	GameObj(PScene owner, bool isDisabled = false);

	[[nodiscard]] string			GetTag() const;
	[[nodiscard]] string			GetName() const;
	[[nodiscard]] ChildContainer&	GetChildContainer();
	[[nodiscard]] ScriptContainer&	GetScriptContainer();
	[[nodiscard]] PGameObj			GetParent() const;

	[[nodiscard]] Transform&		GetTransform() const;
	[[nodiscard]] PhysicComponent&	GetPhysicComponent() const;

	[[nodiscard]] bool				IsDisabled() const override;
	[[nodiscard]] bool				IsRoot() const;
	[[nodiscard]] bool				IsUpdateByScene() const;

	// Remove parent and move this object to root
	bool BecomeRootObject();
	// Remove this from root object and from game loop to add it back use BecomeRootObject() or add this as a child of a root obj
	bool RemoveFromRoot();

	// Set parent, can set to nullptr to remove from the gameLoop, to become root object use BecomeRootObject()
	void SetParent(PGameObj parent);
	void SetName(const std::string& name);
	void SetTag(const std::string& tag);

	void Load(nlohmann::json& input) override;

	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate() override;

	void OnEnabled() override;
	void OnDisabled() override;

	void OnCollide(CollideEvent& e) override;
	
	PGameObj Clone() const;
protected:
	void OnPhysicUpdate();
private:
	void Destroy() override;

	[[nodiscard]] BaseComponent::Type GetComponentType() const override;

	void SetContainerIterator(std::list<PGameObj>::iterator it);
	std::list<PGameObj>::iterator GetContainerIterator() const;

	void SetIsRoot(bool value);
	void SetParentInternally(PGameObj obj);

	void SetParentDisability(bool value);
	bool GetParentDisability();
private:
	PGameObj _parent = nullptr;
	bool _isParentDisabled = false;
	bool _isRoot = false;

	bool _loaded = false;
	string _tag;
	string _name;

	ChildContainer _children;
	ScriptContainer _scripts;

	mutable PhysicComponent _physic;
	mutable Transform _transform;

	std::list<PGameObj>::iterator _containerIterator;
	static nlohmann::json defaultJson;
};

#define GAMEOBJ_FORMAT_EXCEPT(errorField, errorObj, extraDescription) GameObj::GameObjectFormatException(__LINE__,__FILE__,errorField,errorObj,extraDescription)