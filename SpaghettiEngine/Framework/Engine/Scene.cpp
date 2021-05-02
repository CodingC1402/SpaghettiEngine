#include "Scene.h"
#include "CornException.h"
#include "GameObj.h"
#include "Path.h"
#include <sstream>
#include <fstream>

Scene::Scene(const std::string& path)
	:
	path(path)
{}

Scene::SceneException::SceneException(int line, const char* file, const std::string& description)
	: CornException(line, file),
	_description(std::move(description))
{}

const wchar_t* Scene::SceneException::GetType() const noexcept
{
	return L"o(TヘTo) Scene exception";
}

const wchar_t* Scene::SceneException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString() << std::endl;
	os << "[Exception] "<< _description.c_str() << std::endl;
	
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

Scene::BaseComponent::BaseComponent(PScene owner)
	:
	_owner(owner),
	_isDisabled(false)
{}

Scene::BaseComponent::~BaseComponent()
{
	_owner->_genericComponents.erase(this);
}

void Scene::BaseComponent::Disable()
{
	if (_isDisabled)
		return;
	_isDisabled = true;
	OnDisabled();
}

void Scene::BaseComponent::Enable()
{
	if (!_isDisabled)
		return;
	_isDisabled = false;
	OnEnabled();
}

bool Scene::BaseComponent::IsDisabled()
{
	return _isDisabled;
}

void Scene::BaseComponent::Load(nlohmann::json& input)
{
	if (!_isDisabled)
		OnEnabled();
}

void Scene::BaseComponent::Destroy()
{
	delete this;
}

PScene Scene::BaseComponent::GetOwner()
{
	return _owner;
}

void Scene::Start()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->OnStart();
}

void Scene::Update()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->OnUpdate();
}

void Scene::End()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->OnEnd();
}

void Scene::Load()
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "Scene file ";
		os << path.c_str();
		os << " Doesn't exist";
		throw SCENE_EXCEPTION(os.str());
	}

	constexpr const char* Scripts = "Scripts";
	constexpr const char* GameObjects = "GameObjects";
	try
	{
		_tempComponentContainer = new std::map<CULL, SBaseComponent>();
		
		json jsonFile;
		file >> jsonFile;

		if (jsonFile[GameObjects] == nullptr)
			throw std::exception();

		SBaseComponent newComponent;
		for (std::string objPath; const auto& gameObj : jsonFile[GameObjects])
		{
			newComponent = std::make_shared<GameObj>()
			_genericComponents.emplace()
		}

		file.close();

		delete _tempComponentContainer;
		_tempComponentContainer = nullptr;
	}
	catch (const std::exception&)
	{
		delete _tempComponentContainer;
		_tempComponentContainer = nullptr;
		
		throw SCENE_EXCEPTION(std::string("Field ") + GameObjects + " is wrong\n" 
			+ "[Scene file] " + path);
	}
}

void Scene::Unload()
{
	for (const auto& gameObj : _rootGameObjects)
		gameObj->Destroy();
	_rootGameObjects.clear();
}

Scene::SBaseComponent& Scene::GetComponent(CULL& id)
{
	if (!_tempComponentContainer)
		throw SCENE_EXCEPTION("Trying to get component using id after load");
	BaseComponent* tempPointer = _tempComponentContainer->find(id)->second;
	if (!tempPointer)
		throw SCENE_EXCEPTION("Cannot find the requested id");
	return _genericComponents[tempPointer];
}

void Scene::PromoteGameObjToRoot(PGameObj gameObj)
{
	_rootGameObjects.emplace_back(gameObj);
}

void Scene::DemoteGameObjFromRoot(PGameObj gameObj)
{
	_rootGameObjects.remove(gameObj);
}
