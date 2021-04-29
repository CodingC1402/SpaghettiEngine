#include "Scene.h"
#include "CornException.h"
#include "GameObj.h"
#include "Path.h"
#include <sstream>
#include <fstream>

Scene::Scene(std::string path)
	:
	path(path)
{}

void Scene::RemoveGameObject(PGameObj gameObj)
{
	instances.remove(gameObj);
}

void Scene::AddGameObject(PGameObj gameObj)
{
	instances.push_back(gameObj);
}

void Scene::Instantiate(PGameObj gameObj)
{
	if (gameObj->GetParent())
		throw CORN_EXCEPT_WITH_DESCRIPTION(L"You are trying to instantiate an object with parent");
	const auto newInstance = new GameObj(*gameObj);
	newInstance->ownerScene = this;
	instances.push_back(newInstance);
}

PGameObj Scene::GetObj(UINT index[], UINT size)
{
	auto it = instances.begin();
	std::advance(it, index[0]);
	return (*it)->GetChild(index, 0, size);
}

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

void Scene::Start()
{
	for (const auto& instance : instances)
		instance->Start();
}

void Scene::Update()
{
	for (const auto& instance : instances)
		instance->Update();
}

void Scene::End()
{
	for (const auto& instance : instances)
		instance->End();
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

	static constexpr const char* GameObjects = "GameObjects";
	try
	{
		json jsonFile;
		file >> jsonFile;

		if (jsonFile[GameObjects] == nullptr)
			throw std::exception();
		for (std::string objPath; const auto& gameObj : jsonFile[GameObjects])
		{
			objPath = CLib::ConvertPath(path, gameObj.get<std::string>());
			instances.push_back(new GameObj(objPath, this));
		}

		file.close();
	}
	catch (const std::exception&)
	{
		throw SCENE_EXCEPTION(std::string("Field ") + GameObjects + " is wrong\n" 
			+ "[Scene file] " + path);
	}

	for (const auto& instance : instances)
		instance->Load();
}

void Scene::Unload()
{
	for (const auto& instance : instances)
		delete instance;
	instances.clear();
}
