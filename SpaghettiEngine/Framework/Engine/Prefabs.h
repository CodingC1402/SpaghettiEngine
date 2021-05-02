#pragma once
#include "ResourceContainer.h"
#include "json.hpp"
#include <map>

class Prefab : public Resource
{
protected:
	struct ComponentJsonObject
	{
	public:
		ComponentJsonObject(nlohmann::json& jsonObject, const bool& isGameObject);
	public:
		nlohmann::json _jsonObject;
		bool _isGameObject;
	};
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="out"> the scene json object that you would like to append base on this prefab</param>
	/// <param name="index"> index of this prefab according to your </param>
	/// <param name="changes"> the json object that describe what need to be change to the append</param>
	void Append(nlohmann::json& out, unsigned int index, nlohmann::json& changes);
	void Load(const std::string& path) override;
protected:
	std::map<unsigned int, ComponentJsonObject> _components;
};

class PrefabsContainer : public Container<Prefab>
{
	friend class SceneManager;
public:
	PrefabsContainer();
};