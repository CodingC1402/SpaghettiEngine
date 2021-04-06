#pragma once
#include <memory>
#include <vector>
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
	void virtual Update();
	const char* GetTag();
	void SetTag(const char* tag);
	const char* GetName();
	void SetName(const char* name);
protected:
	SSprite sprite;
	SAnimation animation;

	std::string name;
	std::string tag;

	std::vector<SScriptBase> scripts;
};

class ScriptBase
{
public:
	virtual void Update() = 0;
protected:
	PGameObj owner;
};