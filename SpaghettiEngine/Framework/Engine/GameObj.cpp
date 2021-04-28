#include "GameObj.h"
#include "json.hpp"
#include "CornException.h"
#include "SceneManager.h"
#include "GraphicsMath.h"
#include "ScriptBase.h"
#include "Path.h"
#include <fstream>

#pragma region Get
Matrix GameObj::GetWorldMatrix()
{
	CalculateWorldMatrix();
	return _worldMatrix;
}
Vector3 GameObj::GetWorldTransform() const
{
	if (parent != nullptr)
		return _transform + parent->GetWorldTransform();
	else
		return _transform;
}
Vector3 GameObj::GetWorldRotation() const
{
	if (parent != nullptr)
		return _rotation + parent->GetWorldRotation();
	else
		return _rotation;
}
Vector3 GameObj::GetWorldScale() const
{
	Vector3 rValue = _scale;
	if (parent != nullptr)
	{
		const Vector3 parentValue = parent->GetWorldScale();
		rValue = Vector3(rValue.x * parentValue.x, rValue.y * parentValue.y, rValue.y * parentValue.y);
	}
	return rValue;
}
Vector3 GameObj::GetTransform() const
{
	return _transform;
}
Vector3 GameObj::GetRotation() const
{
	return _rotation;
}
Vector3 GameObj::GetScale() const
{
	return _scale;
}
PGameObj GameObj::GetParent() const
{
	return parent;
}
PGameObj GameObj::GetChild(UINT index) const
{
	if (index >= children.size())
		return nullptr;

	auto iterator = children.begin();
	std::advance(iterator, index);
	return *iterator;
}
PGameObj GameObj::GetChild(UINT index[], UINT level, UINT size) const
{
	auto iterator = children.begin();
	std::advance(iterator, index[level]);
	
	if (level == size - 1)
		return *iterator;
	else
		return (*iterator)->GetChild(index, level + 1, size);
}
PScriptBase GameObj::GetScript(UINT index) const noexcept
{
	if (index >= scripts.size())
		return nullptr;

	auto iterator = scripts.begin();
	std::advance(iterator, index);
	return *iterator;
}
PScriptBase GameObj::GetScript(const std::string& name) const noexcept
{
	for (const auto& script : scripts)
	{
		if (script->GetName() == name)
			return script;
	}
	return nullptr;
}
std::list<PScriptBase> GameObj::GetAllScripts(const std::string& name) const noexcept
{
	std::list<PScriptBase> rList;
	for (const auto& script : scripts)
	{
		if (script->GetName() == name)
			rList.push_back(script);
	}
	return rList;
}
std::string GameObj::GetTag() const
{
	return tag;
}
std::string GameObj::GetPath() const
{
	return path;
}
bool GameObj::IsDisabled() const
{
	return isDisabled;
}
#pragma endregion
#pragma region Set
void GameObj::SetTag(const char* newTag)
{
	this->tag = newTag;
}
void GameObj::SetRotation(const float& x, const float& y, const float& z)
{
	SetRotation(Vector3(x, y, z));
}
void GameObj::SetRotation(const Vector3& vec3)
{
	if (_rotation == vec3)
	{
		_rotation = vec3;
		GraphicsMath::Modulo(_rotation, 360.0f);
		_isRotationChanged = true;
		ForceRecalculateMatrix();
	}
}
void GameObj::SetScale(const float& x, const float& y, const float& z)
{
	SetScale(Vector3(x, y, z));
}
void GameObj::SetScale(const Vector3& vec3)
{
	if (_scale != vec3)
	{
		_scale.x = vec3.x;
		_scale.y = vec3.y;
		_scale.z = vec3.z;
		_isScaleChanged = true;
		ForceRecalculateMatrix();
	}
}
void GameObj::SetTransform(const float& x, const float& y, const float& z)
{
	SetTransform(Vector3(x, y, z));
}
void GameObj::SetTransform(const Vector3& vec3)
{
	if (_transform != vec3)
	{
		_transform = vec3;
		_isTransformChanged = true;
		ForceRecalculateMatrix();
	}
}
void GameObj::Rotate(const float& x, const float& y, const float& z)
{
	Rotate(Vector3(x, y, z));
}
void GameObj::Rotate(const Vector3& rotation)
{
	if (rotation == Vector3(0, 0, 0))
		return;
	
	_rotation += rotation;
	GraphicsMath::Modulo(_rotation, 360.0f);
	_isRotationChanged = true;
	ForceRecalculateMatrix();
}
void GameObj::Translate(const float& x, const float& y, const float& z)
{
	Translate(Vector3(x, y, z));
}
void GameObj::Translate(const Vector3& vector)
{
	if (vector == Vector3(0, 0, 0))
		return;
	
	_transform += vector;
	_isTransformChanged = true;
	ForceRecalculateMatrix();
}
void GameObj::ForceRecalculateMatrix()
{
	this->_isChanged = true;
	for (const auto& child : children)
		child->_isChanged = true;
}
#pragma endregion
#pragma region Obj Event
void GameObj::Start()
{
	if (isDisabled)
		return;

	for (const auto& script : scripts)
		script->Start();

	for (const auto& child : children)
		child->Start();
}
void GameObj::Update()
{
	if (isDisabled)
		return;


	for (const auto& script : scripts)
		script->Update();

	for (const auto& child : children)
		child->Update();
}
void GameObj::End() const
{
	for (const auto& script : scripts)
		script->End();
}
#pragma endregion
#pragma region Enable/Disable
void GameObj::Disable()
{
	if (isDisabled)
		return;

	isDisabled = true;
	for (const auto& script : scripts)
		script->OnDisabled();

	for (const auto& child : children)
		child->Disable();
}
void GameObj::Enable()
{
	if (!isDisabled)
		return;

	isDisabled = false;
	for (const auto& script : scripts)
		script->OnEnabled();

	for (const auto& child : children)
		child->Enable();
}
#pragma endregion 
#pragma region Parent methods
void GameObj::RemoveParent()
{
	if (ownerScene)
		ownerScene->AddGameObject(this);

	_transform += parent->GetWorldTransform();
	_rotation  += parent->GetWorldRotation();
	const Vector3 parentScale = parent->GetWorldScale();
	_scale.x *= parentScale.x;
	_scale.y *= parentScale.y;
	_scale.z *= parentScale.z;

	_isTransformChanged = true;
	_isRotationChanged = true;
	_isScaleChanged = true;
	ForceRecalculateMatrix();
	
	parent->RemoveChild(this);
	parent = nullptr;
}
void GameObj::AddParent(const PGameObj& gameObj)
{
	if (ownerScene)
		ownerScene->RemoveGameObject(this);

	parent = gameObj;

	_transform -= parent->GetWorldTransform();
	_rotation  -= parent->GetWorldRotation();
	const Vector3 parentScale = parent->GetWorldScale();
	_scale.x /= parentScale.x;
	_scale.y /= parentScale.y;
	_scale.z /= parentScale.z;

	_isTransformChanged = true;
	_isRotationChanged = true;
	_isScaleChanged = true;
	ForceRecalculateMatrix();
	
	parent->AddChild(this);
}
#pragma endregion
void GameObj::BecomeCurrentSceneObj()
{
	if (ownerScene)
		ownerScene->RemoveGameObject(this);

	SceneManager::GetCurrentScene()->AddGameObject(this);
}
void GameObj::BecomeConstSceneObj()
{
	if (ownerScene)
		ownerScene->RemoveGameObject(this);

	SceneManager::GetConstScene()->AddGameObject(this);
}

GameObj::~GameObj()
{
	for (const auto& child : children)
		child->End();
	for (const auto& child : children)
		delete child;
	children.clear();

	for (auto& script : scripts)
	{
		script->Unload();
		script->Destroy();
	}
	scripts.clear();
}
#pragma region Scripts
void GameObj::AddScript(const std::string& scriptName, const std::string& arg)
{
	PScriptBase newScript = ScriptFactory::CreateInstance(scriptName);
	newScript->owner = this;
	newScript->Load(&arg);
	scripts.push_back(newScript);
}
void GameObj::AddScript(const PScriptBase& script)
{
	scripts.push_back(script);
}
#pragma endregion 
#pragma region Constructors
GameObj::GameObj(const GameObj& obj)
	:
	ownerScene(nullptr),
	parent(obj.parent),
	loaded(obj.loaded),
	path(obj.path),
	tag(obj.tag),
	_transform(obj._transform),
	_rotation(obj._rotation),
	_scale(obj._scale),
	_transformMatrix(obj._transformMatrix),
	_rotationMatrix(obj._rotationMatrix),
	_scaleMatrix(obj._scaleMatrix)
{
	for (const auto& child : obj.children)
		children.push_back(new GameObj(*child));

	for (PScriptBase copyScript; const auto& script : obj.scripts)
	{
		copyScript = ScriptFactory::CopyInstance(script);
		copyScript->owner = this;
		scripts.push_back(copyScript);
	}

	loaded = true;
}
GameObj::GameObj(const std::string& path, const PScene& ownerScene)
	:
	ownerScene(ownerScene),
	parent(nullptr)
{
	this->path = path;
	const Vector3 zeroVec = Vector3(0, 0, 0);
	_transform	= zeroVec;
	_rotation	= zeroVec;
	_scale		= zeroVec;
	
	GraphicsMath::ZeroMatrix(&_transformMatrix);
	GraphicsMath::ZeroMatrix(&_rotationMatrix);
	GraphicsMath::ZeroMatrix(&_scaleMatrix);
	GraphicsMath::ZeroMatrix(&_worldMatrix);
}
#pragma endregion
void GameObj::Load()
{
	if (loaded)
		return;

	loaded = true;
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << L"Obj file ";
		os << path.c_str();
		os << L" Doesn't exist";
		throw CORN_EXCEPT_WITH_DESCRIPTION (os.str());
	}

	constexpr const char* Tag = "Tag";
	constexpr const char* Transform = "Transform";
	constexpr const char* Rotation = "Rotation";
	constexpr const char* Scale = "Scale";
	constexpr const char* Scripts = "Scripts";
	constexpr const char* Children = "Children";
	
	json jsonFile;
	file >> jsonFile;
#pragma region Load prefab
	if (constexpr const char* Prefab = "Prefab"; jsonFile[Prefab] != nullptr)
	{
		const auto prefabPath = jsonFile[Prefab].get<std::string>();
		std::ifstream prefab(prefabPath);
		json jsonPrefab;
		prefab >> jsonPrefab;
		if (!prefab.is_open())
		{
			std::wostringstream os;
			os << L"Prefab file ";
			os << path.c_str();
			os << L" Doesn't exist";
			throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
		}

		try
		{
			if (jsonFile[Transform] == nullptr)
				jsonFile[Transform] = jsonPrefab[Transform];
			if (jsonFile[Rotation] == nullptr)
				jsonFile[Rotation] = jsonPrefab[Rotation];
			if (jsonFile[Scale] == nullptr)
				jsonFile[Scale] = jsonPrefab[Scale];
			if (jsonFile[Tag] == nullptr)
				jsonFile[Tag] = jsonPrefab[Tag];
			for (const auto& script : jsonPrefab[Scripts])
				jsonFile[Scripts].push_back(script);

			std::string mainPrefabPath = CLib::GetPath(prefabPath);
			for (std::string childPath; const auto& child : jsonPrefab[Children])
			{
				//Convert relative childPath to absolute path
				childPath = CLib::ConvertPath(prefabPath, child.get<std::string>());
				jsonFile[Children].push_back(childPath);
			}
		}
		catch (const std::exception& e)
		{
			std::wostringstream os;
			os << L"Prefab file ";
			os << prefabPath.c_str();
			os << L" doesn't have the right format for" << std::endl;
			os << L"Obj file" << path.c_str() << std::endl;
			os << L"Exception: " << e.what();

			throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
		}
	}
#pragma endregion
#pragma region  GameObj
	try
	{
		tag = jsonFile[Tag].get<std::string>();

		_transform.x = jsonFile[Transform][0].get<float>();
		_transform.y = jsonFile[Transform][1].get<float>();
		_transform.z = jsonFile[Transform][2].get<float>();
		
		_rotation.x		= jsonFile[Rotation][0].get<float>();
		_rotation.y		= jsonFile[Rotation][1].get<float>();
		_rotation.z		= jsonFile[Rotation][2].get<float>();

		_scale.x		= jsonFile[Scale][0].get<float>();
		_scale.y		= jsonFile[Scale][1].get<float>();
		_scale.z		= jsonFile[Scale][2].get<float>();

		_isTransformChanged = true;
		_isRotationChanged	= true;
		_isScaleChanged		= true;
		_isChanged			= true;

		std::string childPath;
		const std::string mainPath = CLib::GetPath(GetPath());
		for (PGameObj newChild; const auto& child : jsonFile[Children])
		{
			//Convert relative childPath to absolute path
			childPath = CLib::ConvertPath(path, child.get<std::string>());
			
			newChild = new GameObj(childPath, ownerScene);
			this->AddChild(newChild);
			newChild->parent = this;
			newChild->Load();
		}

		for (const auto& script : jsonFile[Scripts])
		{
			AddScript(script["Name"].get<std::string>(), script["Input"].get<std::string>());
		}

		file.close();
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" doesn't have the right format" << std::endl;
		os << L"Exception: " << e.what();

		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
#pragma endregion 
}
void GameObj::Destroy()
{
	End();

	if (parent)
		parent->RemoveChild(this);
	else if (ownerScene)
		ownerScene->RemoveGameObject(this);
	
	delete this;
}
#pragma region  Child
void GameObj::AddChild(PGameObj child)
{
	children.push_back(child);
}
void GameObj::RemoveChild(PGameObj child)
{
	children.remove(child);
}
#pragma endregion 
#pragma region Matrix Calculation
void GameObj::CalculateRotationMatrix()
{
	if (!_isRotationChanged)
		return;
	
	const Matrix XAxis = GraphicsMath::GetXAxisRotateMatrix(_rotation.x);
	const Matrix YAxis = GraphicsMath::GetYAxisRotateMatrix(_rotation.y);
	const Matrix ZAxis = GraphicsMath::GetZAxisRotateMatrix(_rotation.z);
	
	_rotationMatrix = XAxis * ZAxis * YAxis;
	_isRotationChanged = false;
}
void GameObj::CalculateTransformMatrix()
{
	if (!_isTransformChanged)
		return;

	_transformMatrix._41 = _transform.x;
	_transformMatrix._42 = _transform.y;
	_transformMatrix._43 = _transform.z;
	_transformMatrix._11 = 1;
	_transformMatrix._22 = 1;
	_transformMatrix._33 = 1;
	_transformMatrix._44 = 1;
	_isTransformChanged = false;
}
void GameObj::CalculateScaleMatrix()
{
	if (!_isScaleChanged)
		return;

	_scaleMatrix._11 = _scale.x;
	_scaleMatrix._22 = _scale.y;
	_scaleMatrix._33 = _scale.z;
	_scaleMatrix._44 = 1;
	_isScaleChanged = false;
}
void GameObj::CalculateWorldMatrix()
{
	if (!_isChanged)
		return;
	
	CalculateRotationMatrix();
	CalculateTransformMatrix();
	CalculateScaleMatrix();
	
	if (parent != nullptr)
		_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix * parent->GetWorldMatrix();
	else
		_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix;

	_isChanged = false;
}
#pragma endregion