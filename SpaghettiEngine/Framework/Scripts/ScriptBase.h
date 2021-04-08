#pragma once

#include "GameObj.h"
#include <string>
#include <map>


class ScriptBase;

typedef class GameObj* PGameObj;
typedef std::map<std::string, void* (*)()> ScriptTypes;

template<typename T>
void* CreateT() { return new T; }

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
struct DerivedRegister : public ScriptFactory {
	DerivedRegister(std::string const& scriptName) 
	{
		GetMap()->insert(std::make_pair(scriptName, &CreateT<T>));
	}
};

#define REGISTER_START(NAME) static DerivedRegister<NAME> reg
#define REGISTER_FINISH(NAME) DerivedRegister<NAME> NAME::reg(#NAME)

class ScriptBase
{
	friend class GameObj;
	friend 	ScriptBase* CreateT();
public:
	ScriptBase() = default;
	virtual void Start() {};
	virtual void Update() {};
	virtual void End() {};
protected:
	virtual void Load(const std::string* inputArg, int argS) {};
	virtual void Unload() {};
protected:
	PGameObj owner;
};
