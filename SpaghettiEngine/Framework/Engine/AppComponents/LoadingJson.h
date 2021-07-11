#pragma once
#include "SMath.h"
#include "json.hpp"
#include "Scene.h"
#include "Prefabs.h"
#include "BaseComponent.h"
#include <unordered_map>

namespace LoadingJson
{
    class Field
    {
    public:
        static bool IsRefField(const std::string& field);
    public:
        //TopLevel
        static constexpr const char* scriptsField = "Scripts";
        static constexpr const char* gameObjectsField = "GameObjects";
        static constexpr const char* prefabsField = "Prefabs";
        //Common
        static constexpr const char* idField = "ID";
        static constexpr const char* inputsField = "Inputs";
        static constexpr const char* isDisabled = "IsDisabled";
        static constexpr const char* isOwnedBySceneField = "OwnedByScene";
        static constexpr const char* nameField = "Name";
        //Prefab
        static constexpr const char* prefabIdField = "PrefabID";
        static constexpr const char* changesField = "Changes";
        static constexpr const char* modeField = "Mode";
        static constexpr const char* levelField = "Level";
        static constexpr const char* fieldField = "Field";
        static constexpr const char* valueField = "Value";
        static constexpr const char* typeField = "Type";
        static constexpr const char* refIndex = "RefIndex";
        //Script
        static constexpr const char* scriptTypeField = "ScriptType";
        //GameObj
        static constexpr const char* isRootField = "IsRoot";
        static constexpr const char* isInstantiate = "Instantiate"; // Use for game object to know if they should be instantiate when created or not.
        static constexpr const char* tagField = "Tag";
        static constexpr const char* transformField = "Transform";
        static constexpr const char* rotationField = "Rotation";
        static constexpr const char* scaleField = "Scale";
        //Tile map
        static constexpr const char* textureField = "Texture";
        static constexpr const char* fpsField = "FPS";
        static constexpr const char* spritesField = "Sprites";
        static constexpr const char* animationsField = "Animations";
    protected:
        static std::list<std::string> _refFields;
    };

    class ID
    {
    public:
        static constexpr ULL CreateTopLevelID(CULL& localID, const unsigned& prefabIndex = 0);
        static constexpr ULL CreateLocalLevelID(CULL& id, BaseComponent::Type type);
        static constexpr ULL CreateTopLevelID(CULL& id, BaseComponent::Type type, const unsigned& prefabIndex);
        static constexpr bool CheckID(CULL& id);
        static constexpr bool CheckPrefabIndex(CULL& index);
        static void ConvertIDInJson(nlohmann::json& out, SPrefabHierarchy hierarchy);
        static BaseComponent::Type ConvertStrToType(const std::string& str);
    protected:
        static void ConvertComponentIDs(nlohmann::json& component, BaseComponent::Type type, std::vector<SPrefabHierarchy>& listOfHierarchy);
        static void ConvertRefFieldIDs(nlohmann::json& refField, BaseComponent::Type type, std::vector<SPrefabHierarchy>& listOfHierarchy, const unsigned& defaultID);
    public:
        static constexpr auto _bitForLocalId = 32u;
        static constexpr auto _bitForPrefabId = 16u;
        static constexpr auto _bitForComponentType = 8u;

        static constexpr auto _max = 0xFFFFFFFFFFFFFFFFU;
        static constexpr auto _errorMaskLocalId = _max << _bitForLocalId;
        static constexpr auto _errorMaskPrefabIndex = _max << _bitForPrefabId;
        static constexpr auto _errorMaskComponentType = _max << _bitForComponentType;

        static std::unordered_map<std::string, BaseComponent::Type> _typeDict;
    };
}

constexpr ULL LoadingJson::ID::CreateTopLevelID(CULL& localID, const unsigned& prefabIndex)
{
    return localID | (static_cast<ULL>(prefabIndex) << (_bitForLocalId + _bitForComponentType));
}

constexpr ULL LoadingJson::ID::CreateLocalLevelID(CULL& id, BaseComponent::Type type)
{
    return id | (static_cast<ULL>(type) << _bitForLocalId);
}

constexpr ULL LoadingJson::ID::CreateTopLevelID(CULL& id, BaseComponent::Type type, const unsigned& prefabIndex)
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
