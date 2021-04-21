#pragma once
#include "ScriptBase.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CornDirectX.h"
#include <memory>
#include <list>

typedef class Scene* PScene;

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObjl;

typedef class ScriptBase* PScriptBase;
typedef class std::shared_ptr<ScriptBase> SScriptBase;
typedef class std::unique_ptr<ScriptBase> UScriptBase;

using std::string;
using std::list;
class GameObj
{
	friend class Scene;
public:
	GameObj(const GameObj& obj);
	GameObj(const std::string& path, const PScene& ownerScene = nullptr);
	void Load();
	void Destroy();

	void Start();
	void Update();
	void End() const;

	[[nodiscard]] PMatrix			GetTransformMatrix();
	[[nodiscard]] PMatrix			GetRotationMatrix();
	[[nodiscard]] PMatrix			GetScaleMatrix();
	[[nodiscard]] PScriptBase		GetScript(const UINT index) const noexcept;
	[[nodiscard]] PScriptBase		GetScript(const std::string* name) const noexcept;
	[[nodiscard]] list<PScriptBase> GetAllScripts(const std::string* name) const noexcept;
	[[nodiscard]] Matrix			GetTransformMatrix() const;
	[[nodiscard]] PGameObj			GetParent() const;
	[[nodiscard]] PGameObj			GetChild(UINT index) const;
	[[nodiscard]] string			GetTag() const;
	[[nodiscard]] string			GetPath() const;
	[[nodiscard]] Vector3			GetPosition() const;
	[[nodiscard]] bool				IsDisabled() const;
	void Disable();
	void Enable();

	void Translate(const Vector3& vector);
	void Translate(const float& x, const float& y, const float& z);
	void Move(const Vector3& newPosition);

	void RemoveParent();
	void AddParent(const PGameObj& gameObj);

	void BecomeCurrentSceneObj();
	void BecomeConstSceneObj();

	void SetTag(const char* tag);
protected:
	void AddScript(const std::string& scriptName, const std::string& arg);
	void AddScript(const PScriptBase& script);
	void AddChild(PGameObj child);

	void CalculateWorldMatrix();
	void RemoveChild(PGameObj child);
protected:
	PScene ownerScene;
	PGameObj parent;
	list<PGameObj> children;

	bool isDisabled = false;
	bool loaded = false;
	string path; 
	string tag;

	Vector3 _rotation; // In degree
	Matrix _scaleMatrix;
	Matrix _rotationMatrix;
	Matrix _transformMatrix;
	Matrix _worldMatrix;

	list<PScriptBase> scripts;
};