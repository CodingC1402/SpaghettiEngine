#pragma once
#include <memory>
#include <list>
#include "Sprite.h"
#include "Animation.h"
#include "ScriptBase.h"
#include "Scene.h"

typedef class Scene* PScene;

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObjl;

typedef class ScriptBase* PScriptBase;
typedef class std::shared_ptr<ScriptBase> SScriptBase;
typedef class std::unique_ptr<ScriptBase> UScriptBase;

class GameObj
{
public:
	GameObj(const std::string path, const PScene ownerScene = nullptr);
	void Load();
	void Destroy();

	void Start();
	void Update();
	void End();

	const char* GetTag();
	const char* GetPath();

	void RemoveParent();
	void AddParent(const PGameObj& gameObj);

	void SetTag(const char* tag);
protected:
	~GameObj();
	void AddScript(const std::string& scriptName, const std::string* arg);
	void AddChild(PGameObj obj);
	void RemoveChild(PGameObj obj);
protected:
	PScene ownerScene;
	PGameObj parent;
	std::list<PGameObj> children;

	bool loaded = false;
	std::string path;
	std::string tag;

	std::list<SScriptBase> scripts;
};