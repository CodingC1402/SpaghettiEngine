#include "Scene.h"
#include "json.hpp"
#include "CornException.h"
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
		throw CORN_EXCEPT_WITH_DISCRIPTION(L"You are trying to instantiate an object with parent");
	PGameObj newInstance = new GameObj(*gameObj);
	newInstance->ownerScene = this;
	instances.push_back(newInstance);
}

void Scene::Update()
{
	size_t size = instances.size();
	auto iterator = instances.begin();
	for (int i = 0; i < size; i++)
	{
		(*iterator)->Update();
		std::advance(iterator, 1);
		size--;
	}
}

#define SIZE "Size"

bool Scene::Load()
{
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" Doesn't exist";
		throw CORN_EXCEPT_WITH_DISCRIPTION(os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		int size = jsonFile[SIZE].get<int>();
		char index;
		for (int i = 0; i < size; i++)
		{
			index = static_cast<char>('0' + i);
			instances.push_back(new GameObj(path, this));
		}

		size_t instanceSize = instances.size();
		auto iterator = instances.begin();
		while (instanceSize > 0)
		{
			(*iterator)->Load();
			std::advance(iterator, 1);
			instanceSize--;
		}
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" doesn't have the right format";
		throw CORN_EXCEPT_WITH_DISCRIPTION(os.str());
	}

	size_t startSize = instances.size();
	auto itStart = instances.begin();
	while (startSize > 0)
	{
		(*itStart)->Start();
		std::advance(itStart, 1);
		startSize--;
	}
}

void Scene::Unload()
{
	size_t instanceSize = instances.size();
	auto iterator = instances.begin();
	while (instanceSize > 0)
	{
		(*iterator)->Destroy();
		std::advance(iterator, 1);
		instanceSize--;
	}
}
