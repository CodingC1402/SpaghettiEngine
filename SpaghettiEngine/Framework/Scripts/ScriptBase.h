#pragma once

#include "GameObj.h"
#include "CornDirectX.h"
#include "json.hpp"
#include "CornException.h"
#include "Scene.h"
#include <string>
#include <map>

typedef class ScriptBase* PScriptBase;
typedef const ScriptBase* CPScriptBase;
typedef std::weak_ptr<ScriptBase> WScriptBase;
typedef std::map<std::string, void* (*)(PScene)> ScriptTypes;

template<typename T>
void* CreateT(PScene owner) { return new T(owner); }

class ScriptFactory
{
public:
	static PScriptBase CreateInstance(std::string const& typeName, PScene owner);
	static PScriptBase CopyInstance(CPScriptBase instance);
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

#define REGISTER_START(NAME) static DerivedRegister<NAME> reg
#define REGISTER_FINISH(NAME) DerivedRegister<NAME> NAME::reg(#NAME)
#define TYPE_NAME(TYPE) #TYPE


class ScriptBase : public Scene::BaseComponent
{
	friend class GameObj;
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
	void AssignOwner(const PGameObj& owner);
	
	[[nodiscard]] const char* GetName() const noexcept;
	[[nodiscard]] Matrix	GetWorldMatrix() const noexcept;
	[[nodiscard]] Vector3	GetWorldTransform()	const noexcept;
	[[nodiscard]] Vector3	GetWorldRotation() const noexcept;
	[[nodiscard]] Vector3	GetWorldScale() const noexcept;	
	void Load(nlohmann::json& input) override { }
	Scene::SBaseComponent   Clone() override;

	void Destroy() override;
protected:
	bool _isDisabled = false;
	PGameObj _ownerObj = nullptr;
	std::string _name;
};

#define SCRIPT_FORMAT_EXCEPT(Script, Description) ScriptBase::ScriptException(__LINE__,__FILE__,Script,Description)