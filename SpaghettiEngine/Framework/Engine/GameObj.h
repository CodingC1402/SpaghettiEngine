#pragma once
#include "CornDirectX.h"
#include <memory>
#include <list>
#include <string>

typedef class Scene* PScene;
typedef class ScriptBase* PScriptBase;
typedef const ScriptBase* CPScriptBase;
typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObj;

using std::string;
using std::list;
class GameObj
{
	friend class Scene;
public:
	[[nodiscard]] Matrix			GetWorldMatrix();
	[[nodiscard]] Vector3			GetWorldTransform() const;
	[[nodiscard]] Vector3			GetWorldRotation() const;
	[[nodiscard]] Vector3			GetWorldScale() const;
	[[nodiscard]] Vector3			GetTransform() const;
	[[nodiscard]] Vector3			GetRotation() const;
	[[nodiscard]] Vector3			GetScale() const;
	[[nodiscard]] PScriptBase		GetScript(const UINT index) const noexcept;
	[[nodiscard]] PScriptBase		GetScript(const std::string& name) const noexcept;
	[[nodiscard]] list<PScriptBase> GetAllScripts(const std::string& name) const noexcept;
	[[nodiscard]] PGameObj			GetParent() const;
	[[nodiscard]] PGameObj			GetChild(UINT index) const;
	[[nodiscard]] PGameObj			GetChild(UINT index[], UINT level, UINT size) const;
	[[nodiscard]] string			GetTag() const;
	[[nodiscard]] string			GetPath() const;
	[[nodiscard]] bool				IsDisabled() const;

	void SetTag(const char* tag);
	void SetRotation(const float& x, const float& y, const float& z);
	void SetScale(const float& x, const float& y, const float& z);
	void SetTransform(const float& x, const float& y, const float& z);
	void SetRotation(const Vector3& vec3);
	void SetScale(const Vector3& vec3);
	void SetTransform(const Vector3& vec3);
	void Rotate(const float& x, const float& y, const float& z);
	void Rotate(const Vector3& rotation);
	void Translate(const Vector3& vector);
	void Translate(const float& x, const float& y, const float& z);
	void ForceRecalculateMatrix();
	
	GameObj(const GameObj& obj);
	GameObj(const std::string& path, const PScene& ownerScene = nullptr);
	virtual void Load();
	void Destroy();

	void Start();
	void Update();
	void End() const;
	
	void Disable();
	void Enable();

	void RemoveParent();
	void AddParent(const PGameObj& gameObj);

	void BecomeCurrentSceneObj();
	void BecomeConstSceneObj();
protected:
	~GameObj();
	
	void AddScript(const std::string& scriptName, const std::string& arg);
	void AddScript(const PScriptBase& script);
	void AddChild(PGameObj child);

	void CalculateRotationMatrix();
	void CalculateTransformMatrix();
	void CalculateScaleMatrix();
	void CalculateWorldMatrix();
	
	void RemoveChild(PGameObj child);
protected:
	PScene ownerScene;
	PGameObj parent;
	list<PGameObj> children;

	bool _isTransformChanged = true;
	bool _isRotationChanged = true;
	bool _isScaleChanged = true;
	bool _isChanged = true;
	
	bool isDisabled = false;
	bool loaded = false;
	string path; 
	string tag;

	Vector3 _transform;
	Vector3 _rotation; // In degree
	Vector3 _scale;

	Matrix _transformMatrix;
	Matrix _rotationMatrix;
	Matrix _scaleMatrix;
	Matrix _worldMatrix;

	list<PScriptBase> scripts;
};