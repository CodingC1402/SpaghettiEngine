#pragma once
#include "json.hpp"
#include "CornException.h"
#include "PhysicComponent.h"
#include "Scene.h"
#include "BaseComponent.h"
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
			os << "[Game object] " << _errorObj->GetPath().c_str() << std::endl;
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
	[[nodiscard]] bool				IsDestroyed() const;

	void SetTag(const std::string& tag);

	void AddPhysicComponent(PhysicScriptBase* script);
	void RemovePhysicComponent(PhysicScriptBase* script);
	PhysicComponent& GetPhysicComponent();

	void SendCollideExitEvent(CollideEvent& e);
	void SendCollideEvent(CollideEvent& e);
	void SendCollideEnterEvent(CollideEvent& e);

	void Load(nlohmann::json& input) override;
	void Destroy() override;

	void OnStart() override;
	void OnUpdate() override;
	void OnPhysicUpdate();
	void OnFixedUpdate() override;
	void OnEnd() override;

	void OnEnabled() override;
	void OnDisabled() override;

	void OnCollide(CollideEvent& e) override;
	void OnCollideEnter(CollideEvent& e) override;
	void OnCollideExit(CollideEvent& e) override;

	PScriptBase AddScript(const std::string& scriptName, nlohmann::json& inputObject);
	PScriptBase AddScript(const PScriptBase& script);
	PScriptBase AddScript(const SScriptBase& script);
	PScriptBase AddScriptClone(const PScriptBase& script);

	void ClearScripts(); //Use by scripts to call end once then pass on
	void RemoveScript(const PScriptBase& script);
	
	SGameObj Clone() const;
protected:
	PGameObj parent = nullptr;

	bool _isReadyForDelete = false;

	bool loaded = false;
	string _tag;

	std::deque<PhysicScriptBase*> _physicComponents;
	PhysicComponent _physic;

	list<SScriptBase> _scriptsPtr;
	static nlohmann::json defaultJson;
};

#define GAMEOBJ_FORMAT_EXCEPT(errorField, errorObj, extraDescription) GameObj::GameObjectFormatException(__LINE__,__FILE__,errorField,errorObj,extraDescription)