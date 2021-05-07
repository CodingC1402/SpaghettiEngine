#pragma once
#include "ExMath.h"
#include "json.hpp"
#include "Scene.h"
#include <unordered_map>

namespace LoadingJson
{
	class Field
	{
	public:
		static constexpr bool IsRefField(const char* field);
	public:
		//TopLevel
		static constexpr const char* scriptsField		= "Scripts";
		static constexpr const char* gameObjectsField	= "GameObjects";
		static constexpr const char* prefabsField		= "Prefabs";
		//Common
		static constexpr const char* idField			= "ID";
		static constexpr const char* inputsField		= "Inputs";
		static constexpr const char* isDisabled			= "IsDisabled";
		//Prefab
		static constexpr const char* prefabIdField		= "PrefabID";
		static constexpr const char* changesField		= "Changes";
		static constexpr const char* levelField			= "Level";
		static constexpr const char* fieldField			= "Field";
		static constexpr const char* valueField			= "Value";
		static constexpr const char* typeField			= "Type";
		static constexpr const char* refIndex			= "RefIndex";;
		//Script
		static constexpr const char* scriptTypeField	= "ScriptType";
		//GameObj
		static constexpr const char* isRootField		= "IsRoot";
		static constexpr const char* tagField			= "Tag";
		static constexpr const char* transformField		= "Transform";
		static constexpr const char* rotationField		= "Rotation";
		static constexpr const char* scaleField			= "Scale";
	protected:
		static inline std::list<const char*> _refFields = {
			gameObjectsField,
			scriptsField
		};
	};
	
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


constexpr bool LoadingJson::Field::IsRefField(const char* field)
{
	for (const auto& refField : _refFields)
		if (refField == field)
			return true;
	return false;
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
	return !(index & _errorMaskPrefabIndex);
}
