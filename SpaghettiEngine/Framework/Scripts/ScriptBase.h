#pragma once

#include "GameObj.h"
#include <string>
#include <map>

typedef class GameObj* PGameObj;
typedef std::map<std::string, ScriptBase* (*)()> ScriptTypes;

class ScriptFactory
{
public:
	static ScriptBase* CreateInstance(std::string const& typeName);
protected:
	static ScriptTypes* GetMap();
private:
	static ScriptTypes* map;
};

template<typename T>
struct DerivedRegister : ScriptFactory {
	DerivedRegister(std::string const& scriptName) {
		GetMap()->insert(std::make_pair(scriptName, &CreateInstance<T>));
	}
};

#define REGISTER_START(NAME) static DerivedRegister<NAME> reg
#define REGISTER_FINISH(NAME) DerivedRegister<NAME> NAME::reg(#NAME)

class ScriptBase
{
	friend class GameObj;
public:
	virtual void Start() {};
	virtual void Update() {};
	virtual void End() {};
protected:
	virtual void Load(const std::string* input) {};
protected:
	PGameObj owner;
};