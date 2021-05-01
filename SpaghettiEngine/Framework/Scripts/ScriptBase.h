#pragma once

#include "GameObj.h"
#include "CornDirectX.h"
#include "json.hpp"
#include "CornException.h"
#include <string>
#include <map>

typedef class ScriptBase* PScriptBase;
typedef const ScriptBase* CPScriptBase;
typedef std::map<std::string, void* (*)()> ScriptTypes;

template<typename T>
void* CreateT() { return new T; }

class ScriptFactory
{
public:
	static PScriptBase CreateInstance(std::string const& typeName);
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

class ScriptBase
{
	friend class GameObj;
	friend 	ScriptBase* CreateT();
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
	ScriptBase() = default;
	[[nodiscard]] const char*		GetName() const noexcept;
	[[nodiscard]] virtual Matrix	GetWorldMatrix() noexcept;
	[[nodiscard]] virtual Vector3	GetTransform()	const noexcept;
	virtual bool Copy(CPScriptBase script);
	virtual void Start() {}
	virtual void Update() {}
	virtual void End() {}
	virtual void Disable();
	virtual void Enable();
	virtual void OnCollision() {}
	virtual void OnDisabled() {}
	virtual void OnEnabled() {}
	void Destroy() const;
protected:
	virtual ~ScriptBase() = default;
	virtual void Load(nlohmann::json& inputObject);
	virtual void Unload();
protected:
	bool isDisabled = false;
	PGameObj owner = nullptr;
	std::string name;
};

#define SCRIPT_FORMAT_EXCEPT(Script, Description) ScriptBase::ScriptException(__LINE__,__FILE__,Script,Description)
//#define SCRIPT_FORMAT_EXCEPT(Script) ScriptBase::ScriptException(__LINE__,__FILE__,Script,"")