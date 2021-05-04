#pragma once
#include "ExMath.h"
#include "json.hpp"
#include "Scene.h"
#include <unordered_map>

namespace LoadingJson
{
	//TopLevel
	constexpr auto scriptsField		= "Scripts";
	constexpr auto gameObjectsField = "GameObjects";
	constexpr auto prefabsField		= "Prefabs";
	//Common
	constexpr auto idField			= "ID";
	constexpr auto inputsField		= "Inputs";
	constexpr auto isDisabled		= "IsDisabled";
	//Prefab
	constexpr auto prefabIdField	= "PrefabID";
	constexpr auto changesField		= "Changes";
	constexpr auto fieldField		= "Field";
	constexpr auto valueField		= "Value";
	constexpr auto typeField		= "Type";
	//Script
	constexpr auto scriptTypeField	= "ScriptType";
	//GameObject
	constexpr auto isRootField		= "IsRoot";
	constexpr auto tagField			= "Tag";
	constexpr auto transformField	= "Transform";
	constexpr auto rotationField	= "Rotation";
	constexpr auto scaleField		= "Scale";
	
	class ID
	{
	public:
		static constexpr ULL CreateTopLevelID(CULL& localID, const unsigned& prefabIndex = 0);
		static constexpr ULL CreateLocalLevelID(CULL& id, Scene::ComponentType type);
		static constexpr ULL CreateTopLevelID(CULL& id, Scene::ComponentType type, const unsigned& prefabIndex);
		static constexpr bool CheckID(CULL& id);
		static constexpr bool CheckPrefabIndex(CULL& index);
		static void ConvertIDInJson(nlohmann::json& out, Scene::ComponentType type);
		static Scene::ComponentType ConvertStrToType(const std::string& str);
	public:
		static constexpr auto _bitForLocalId = 32u;
		static constexpr auto _bitForPrefabId = 16u;
		static constexpr auto _bitForComponentType = 8u;
	
		static constexpr auto _max = 0xFFFFFFFFFFFFFFFFU;
		static constexpr auto _errorMaskLocalId = _max << _bitForLocalId;
		static constexpr auto _errorMaskPrefabIndex = _max << _bitForPrefabId;
		static constexpr auto _errorMaskComponentType = _max << _bitForComponentType;

		static std::unordered_map<std::string, Scene::ComponentType> _typeDict;
	};
}



constexpr ULL LoadingJson::ID::CreateTopLevelID(CULL& localID, const unsigned& prefabIndex)
{
	return localID | (static_cast<ULL>(prefabIndex) << (_bitForLocalId + _bitForComponentType));
}

constexpr ULL LoadingJson::ID::CreateLocalLevelID(CULL& id, Scene::ComponentType type)
{
	return id | (static_cast<ULL>(type) << _bitForLocalId);
}

constexpr ULL LoadingJson::ID::CreateTopLevelID(CULL& id, Scene::ComponentType type, const unsigned& prefabIndex)
{
	return id | (static_cast<ULL>(type) << _bitForLocalId) | (static_cast<ULL>(prefabIndex) << (_bitForLocalId + _bitForComponentType));
}

constexpr bool LoadingJson::ID::CheckID(CULL& id)
{
	return !(id & _errorMaskLocalId);
}

constexpr bool LoadingJson::ID::CheckPrefabIndex(CULL& index)
{
	return index == 0 || !(index & _errorMaskPrefabIndex);
}
