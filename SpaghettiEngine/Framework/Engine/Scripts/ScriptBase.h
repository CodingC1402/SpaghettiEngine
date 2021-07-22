#pragma once

#include "Vector3.h"
#include "json.hpp"
#include "CornException.h"
#include "GameObj.h"
#include "Scene.h"
#include "BaseComponent.h"

#include <string>
#include <unordered_map>

typedef class ScriptBase* PScriptBase;
typedef const ScriptBase* CPScriptBase;
typedef std::weak_ptr<ScriptBase> WScriptBase;
typedef std::unordered_map<std::string, void* (*)(PScene, bool)> ScriptTypes;

typedef class GameObj* PGameObj;

template<typename T>
void* CreateT(PScene owner, bool isDisabled) { return new T(owner, isDisabled); }

class ScriptFactory
{
public:
	static PScriptBase CreateInstance(std::string const& typeName, PScene owner, bool isDisabled = false);
protected:
	static ScriptTypes* GetMap();
private:
	static ScriptTypes* map;
};

template<typename T>
struct DerivedRegister : public ScriptFactory {
	DerivedRegister(std::string const& scriptName) 
	{
		GetMap()->insert(std::make_pair(scriptName, &CreateT<T>));
	}
};

#define REGISTER_START(NAME) protected: std::string GetType() const noexcept override; public: NAME(PScene owner, bool isDisabled = false); private: static DerivedRegister<NAME> reg
// Register finish with constructor
#define REGISTER_FINISH(NAME, BASECLASS) std::string NAME::GetType() const noexcept { return #NAME; } DerivedRegister<NAME> NAME::reg(#NAME); NAME::NAME(PScene owner, bool isDisabled) : BASECLASS(owner, isDisabled)
#define TYPE_NAME(TYPE) #TYPE

// Get first script of that type from parent game object of the game object owner.
#define GET_FIRST_SCRIPT_OF_TYPE_FROM_PARENT(ScriptType) dynamic_cast<ScriptType*>(GetGameObject()->GetParent()->GetScriptContainer().GetItemType(#ScriptType))
#define GET_FIRST_SCRIPT_OF_TYPE(ScriptType) dynamic_cast<ScriptType*>(GetGameObject()->GetScriptContainer().GetItemType(#ScriptType))
#define GET_ALL_SCRIPTS_OF_TYPE(ScriptType) GetGameObject()->GetScriptContainer().GetAllItemsType(#ScriptType)

class ScriptBase : public BaseComponent
{
	friend class GameObj;
	friend class ScriptContainer;
public:
	class ScriptException : public CornException
	{
	public:
		ScriptException(int line, const char* file, PScriptBase errorScript, const std::wstring& extraDescription);
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
	protected:
		PScriptBase _errorScript;
		std::wstring _extraDescription;
	};
public:
	ScriptBase(PScene owner, bool isDisabled = false);

	virtual void SetGameObject(const PGameObj& owner);
	
	[[nodiscard]] Matrix4	GetWorldMatrix() const noexcept;
	[[nodiscard]] Vector3	GetWorldTransform()	const noexcept;
	[[nodiscard]] Vector3	GetWorldRotation() const noexcept;
	[[nodiscard]] Vector3	GetWorldScale() const noexcept;	
	[[nodiscard]] PGameObj	GetGameObject() const noexcept;
	[[nodiscard]] bool		IsDisabled() const noexcept;

	[[nodiscard]] virtual std::string GetType() const noexcept = 0;
	[[nodiscard]] virtual PScriptBase Clone() const;

	void Load(nlohmann::json& input) override;
private:
	[[nodiscard]] BaseComponent::Type GetComponentType() const override;
	void Destroy() override;

	void SetContainerIterator(std::list<PScriptBase>::iterator it);
	std::list<PScriptBase>::iterator GetContainerIterator() const;
private:
	PGameObj _ownerObj = nullptr;
	std::list<PScriptBase>::iterator _containerIterator;
};

#define SCRIPT_FORMAT_EXCEPT(Script, Description) ScriptBase::ScriptException(__LINE__,__FILE__,Script,Description)