#include "Scene.h"
#include "CornException.h"
#include "GameObj.h"
#include <sstream>
#include <fstream>

Scene::Scene(std::string path)
	:
	path(path)
{}

void Scene::RemoveGameObject(PGameObj gameObj)
{
	size_t size = instances.size();
	auto iterator = instances.begin();
	while (size > 0)
	{
		if ((*iterator) == gameObj)
		{
			instances.erase(iterator);
			return;
		}
		std::advance(iterator, 1);
		size--;
	}
	return;
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

#define GAMEOBJS "GameObjects"
#define TILEMAP "TileMap"
void Scene::Load()
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" Doesn't exist";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		for (const auto& gameObj : jsonFile[GAMEOBJS])
		{
			instances.push_back(new GameObj(gameObj.get<std::string>(), this));
		}

		file.close();
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" doesn't have the right format";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
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
