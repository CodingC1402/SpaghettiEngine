#pragma once
#include <memory>
#include <list>
#include "Sprite.h"
#include "Animation.h"

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObjl;

typedef class ScriptBase* PScriptBase;
typedef class std::shared_ptr<ScriptBase> SScriptBase;
typedef class std::unique_ptr<ScriptBase> UScriptBase;



class GameObj
{
public:
	void Start();
	void Update();
	void End();

	const char* GetTag();
	const char* GetName();

	void SetTag(const char* tag);
	void SetName(const char* name);
protected:
	~GameObj();
	void Load(const std::string& path);
protected:
	PGameObj parent;
	std::list<PGameObj> children;

	std::string name;
	std::string tag;

	std::list<SScriptBase> scripts;
};

class ScriptBase
{
	friend class GameObj;
public:
	virtual void Start() {};
	virtual void Update() {};
	virtual void End() {};
private:
	virtual void Load(const std::string* input) {};
protected:
	PGameObj owner;

};