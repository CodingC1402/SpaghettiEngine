#pragma once
#include "json.hpp"
#include "CornException.h"
#include "PhysicComponent.h"
#include "Scene.h"
#include "BaseComponent.h"
#include "ChildContainer.h"
#include "Transform.h"

#include <memory>
#include <list>
#include <string>

typedef class PhysicScriptBase* PPhysicScriptBase;

CLASS_FORWARD_DECLARATION(ScriptBase);
CLASS_FORWARD_DECLARATION(GameObj);

using std::string;
using std::list;
class GameObj : public BaseComponent
{
	friend class Scene;
	friend class PhysicComponent;
	friend class ChildContainer;
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
	GameObj(GameObj& obj);
	GameObj& operator= (GameObj& obj);

	[[nodiscard]] PScriptBase		GetScript(const unsigned index) const noexcept;
	[[nodiscard]] PScriptBase		GetScript(const std::string& name) const noexcept;
	[[nodiscard]] list<PScriptBase> GetAllScripts(const std::string& name) const noexcept;
	[[nodiscard]] string			GetTag() const;
	[[nodiscard]] string			GetName() const;
	[[nodiscard]] ChildContainer&	GetChildContainer();
	[[nodiscard]] PGameObj			GetParent();

	[[nodiscard]] Transform&		GetTransform();
	[[nodiscard]] PhysicComponent&	GetPhysicComponent();

	[[nodiscard]] bool				IsDestroyed() const;

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
	void OnCollideEnter(CollideEvent& e) override;
	void OnCollideExit(CollideEvent& e) override;
	
	SGameObj Clone() const;
protected:
	void OnPhysicUpdate();
private:
	void Destroy() override;
protected:
	PGameObj _parent = nullptr;

	bool _isReadyForDelete = false;

	bool _loaded = false;
	string _tag;
	string _name;

	ChildContainer _children;
	PhysicComponent _physic;
	Transform _transform;

	list<WScriptBase> _scripts;

	static nlohmann::json defaultJson;
};

#define GAMEOBJ_FORMAT_EXCEPT(errorField, errorObj, extraDescription) GameObj::GameObjectFormatException(__LINE__,__FILE__,errorField,errorObj,extraDescription)