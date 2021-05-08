#pragma once
#include "ResourceContainer.h"
#include "json.hpp"
#include <map>
#include <memory>
#include <vector>

class PrefabHierarchy;
typedef std::shared_ptr<PrefabHierarchy> SPrefabHierarchy;

class PrefabHierarchy
{
public:
	static void ConstructVectorOfHierarchy(std::vector<SPrefabHierarchy>& out, SPrefabHierarchy root);
	
	PrefabHierarchy(int numberOfChild, int value);
	void AddChild(std::shared_ptr<PrefabHierarchy>& child);
	[[nodiscard]] unsigned int GetIndex(const std::vector<unsigned>& accessIndexes) const;
	void ConstructVectorOfChildHierarchy(std::vector<SPrefabHierarchy>& out) const;
protected:
	[[nodiscard]] unsigned int GetIndexRecursive(const std::vector<unsigned>& accessIndexes, unsigned level) const;
protected:
	std::vector<std::shared_ptr<PrefabHierarchy>> _children;
	unsigned int _value = 0;
};

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
	SPrefabHierarchy Append(nlohmann::json& out, unsigned int& index, nlohmann::json& changes) const;
	void Load(const std::string& path) override;
public:
	static constexpr unsigned long long acceptMask = 0xFFFFFFFFFFFFFFFFU;
protected:
	std::map<CULL, ComponentJsonObject> _components;
	std::list<CULL> _subPrefabsIDs;
	nlohmann::json _subPrefabs;
};

class PrefabsContainer : public Container<Prefab>
{
	friend class SceneManager;
public:
	PrefabsContainer();
};