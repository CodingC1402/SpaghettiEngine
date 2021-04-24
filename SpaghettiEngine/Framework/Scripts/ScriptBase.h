#pragma once

#include "GameObj.h"
#include <string>
#include <map>


typedef class ScriptBase* PScriptBase;
typedef const ScriptBase* CPScriptBase;

typedef class GameObj* PGameObj;
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
	ScriptBase() = default;
	[[nodiscard]] const char* GetName() const;
	virtual bool Copy(CPScriptBase script);
	virtual void Start() {}
	virtual void Update() {}
	virtual void End() {}
	virtual void Disable();
	virtual void Enable();
	virtual void OnCollision() {}
	virtual void OnDisabled() {}
	virtual void OnEnabled() {}
	void Destroy();
protected:
	virtual ~ScriptBase() = default;
	virtual void Load(const std::string* inputArg) {}
	virtual void Unload() {}
protected:
	bool isDisabled = false;
	PGameObj owner = nullptr;
	std::string name;
};
