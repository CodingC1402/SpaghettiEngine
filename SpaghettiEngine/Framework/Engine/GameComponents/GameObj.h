#pragma once
#include "json.hpp"
#include "CornException.h"
#include "PhysicComponent.h"
#include "Scene.h"
#include <memory>
#include <list>
#include <string>

typedef class ScriptBase* PScriptBase;
typedef std::shared_ptr<ScriptBase> SScriptBase;

typedef const ScriptBase* CPScriptBase;
typedef class PhysicScriptBase* PPhysicScriptBase;

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObj;
typedef std::weak_ptr<GameObj> WGameObj;

using std::string;
using std::list;
class GameObj : public Scene::BaseComponent
{
	friend class Scene;
	friend class PhysicComponent;
public:
	class GameObjectFormatException : public CornException
	{
	public:
		GameObjectFormatException(int line, const char* file, const char* errorField, PGameObj errorObj, const char* extraDescription)
			: CornException(line, file), _errorField(errorField), _errorObj(errorObj), _extraDescription(extraDescription) {}
		const wchar_t* GetType() const noexcept override
		{
			return L"(」°ロ°)」Game object format exception";
		}
		const wchar_t* What() const noexcept override
		{
			std::wostringstream os;
			os << GetOriginString() << std::endl;
			os << "[Game object] " << _errorObj->GetPath().c_str() << std::endl;
			os << "[Error field] " << _errorField.c_str() << std::endl;
			os << "[Extra description] " << _extraDescription.c_str() << std::endl;
			whatBuffer = os.str();
			return whatBuffer.c_str();
		}
	protected:
		string _errorField;
		PGameObj _errorObj;
		string _extraDescription;
	};
public:
	GameObj(PScene owner, bool isDisabled = false);
	GameObj(GameObj& obj);
	GameObj& operator= (GameObj& obj);

	[[nodiscard]] Matrix4			GetWorldMatrix();
	[[nodiscard]] Vector3			GetWorldTransform() const;
	[[nodiscard]] Vector3			GetWorldRotation() const;
	[[nodiscard]] Vector3			GetWorldScale() const;
	[[nodiscard]] Vector3			GetTransform() const;
	[[nodiscard]] Vector3			GetRotation() const;
	[[nodiscard]] Vector3			GetScale() const;
	[[nodiscard]] PScriptBase		GetScript(const unsigned index) const noexcept;
	[[nodiscard]] PScriptBase		GetScript(const std::string& name) const noexcept;
	[[nodiscard]] list<PScriptBase> GetAllScripts(const std::string& name) const noexcept;
	[[nodiscard]] PGameObj			GetParent() const;
	[[nodiscard]] PGameObj			GetChild(unsigned index) const;
	[[nodiscard]] string			GetTag() const;
	[[nodiscard]] string			GetPath() const;

	void AddPhysicComponent(PhysicScriptBase* script);
	void RemovePhysicComponent(PhysicScriptBase* script);
	PhysicComponent& GetPhysicComponent();

	void SendCollideExitEvent(CollideEvent& e);
	void SendCollideEvent(CollideEvent& e);
	void SendCollideEnterEvent(CollideEvent& e);

	void SetTag(const char* tag);
	void SetRotation(const float& x, const float& y, const float& z);
	void SetScale(const float& x, const float& y, const float& z);
	void SetTransform(const float& x, const float& y, const float& z);
	void SetRotation(const Vector3& vec3);
	void SetScale(const Vector3& vec3);
	void SetTransform(const Vector3& vec3);
	void Rotate(const float& x, const float& y, const float& z);
	void Rotate(const Vector3& rotation);

	void Move(const Vector3& vector);
	void Move(const float& x, const float& y, const float& z);

	void Translate(const Vector3& vector);
	void Translate(const float& x, const float& y, const float& z);

	void ForceRecalculateMatrix();

	void Load(nlohmann::json& input) override;
	void Destroy() override;

	void OnStart() override;
	void OnUpdate() override;
	void OnPhysicUpdate();
	void OnFixedUpdate() override;
	void OnEnd() override;

	void OnEnabled() override;
	void OnDisabled() override;

	void OnCollide(CollideEvent& e) override;
	void OnCollideEnter(CollideEvent& e) override;
	void OnCollideExit(CollideEvent& e) override;

	void RemoveParent();
	void AddParent(const PGameObj& gameObj);
	void AddParentWithoutCalculateLocal(const PGameObj& gameObj);
	void RemoveParentWithoutCalculateLocal();

	PScriptBase AddScript(const std::string& scriptName, nlohmann::json& inputObject);
	PScriptBase AddScript(const PScriptBase& script);
	PScriptBase AddScript(const SScriptBase& script);
	PScriptBase AddScriptClone(const PScriptBase& script);

	PGameObj	AddChild(const PGameObj& child);
	PGameObj	AddChildClone(const PGameObj& child);
	PGameObj	AddChild();

	void RemoveChild(const PGameObj& child);
	void RemoveScript(const PScriptBase& script);
	
	std::shared_ptr<BaseComponent> Clone() override;

	void RecursiveClearScripts(); //Use by scripts to call end once then pass on
	void ClearScripts(); //Use by scripts to call end once then pass on
protected:
	void RecursiveMarkForDelete();

	void RecursiveClearScriptsWithoutCallEnd();
	void ClearScriptsWithoutCallEnd();

	void CalculateRotationMatrix();
	void CalculateTransformMatrix();
	void CalculateScaleMatrix();
	void CalculateWorldMatrix();
protected:
	PGameObj parent = nullptr;
	list<std::shared_ptr<BaseComponent>> _childrenPtr;
	list<PGameObj> _children;

	bool _isReadyForDelete = false;
	
	bool _isTransformChanged = true;
	bool _isRotationChanged = true;
	bool _isScaleChanged = true;
	bool _isChanged = true;
	
	bool loaded = false;
	string path; 
	string tag;

	Vector3 _transform;
	Vector3 _rotation; // In degree
	Vector3 _scale;

	Matrix4 _transformMatrix;
	Matrix4 _rotationMatrix;
	Matrix4 _scaleMatrix;
	Matrix4 _worldMatrix;

	std::deque<PhysicScriptBase*> _physicComponents;

	PhysicComponent _physic;

	list<std::shared_ptr<BaseComponent>> _scriptsPtr;
	list<PScriptBase> _scripts;
	static nlohmann::json defaultJson;
};

#define GAMEOBJ_FORMAT_EXCEPT(errorField, errorObj, extraDescription) GameObj::GameObjectFormatException(__LINE__,__FILE__,errorField,errorObj,extraDescription)