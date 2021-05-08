#include "LoadingJson.h"
#include "Scene.h"
#include "CornException.h"
#include "Prefabs.h"

std::list<std::string> LoadingJson::Field::_refFields{
    gameObjectsField,
    scriptsField
};

std::unordered_map<std::string, Scene::ComponentType> LoadingJson::ID::_typeDict{
    {"GameObject", Scene::ComponentType::gameObj},
    {"Script", Scene::ComponentType::script}
};

bool LoadingJson::Field::IsRefField(const std::string& field)
{
    for (const auto& refField : _refFields)
        if (refField == field)
            return true;
    return false;
}

#pragma region Convert IDs
void LoadingJson::ID::ConvertRefFieldIDs(nlohmann::json& refField, Scene::ComponentType type, std::vector<SPrefabHierarchy>& listOfHierarchy, const unsigned& defaultID)
{
    if (refField.empty())
        return;
    try
    {
        for (unsigned refID; auto & ref : refField)
        {
            if (ref[Field::prefabIdField].empty())
                refID = defaultID;
            else
                refID = ref[Field::prefabIdField].get<unsigned>();

            std::vector<unsigned> prefabsIndexes;
            if (!ref[Field::prefabsField].empty())
            {
                prefabsIndexes.clear();
                for (const auto& prefabIndex : ref[Field::prefabsField])
                    prefabsIndexes.push_back(prefabIndex.get<unsigned>());
                refID = listOfHierarchy[refID]->GetIndex(prefabsIndexes);
            }

            ref[Field::idField] = ID::CreateTopLevelID(ref[Field::idField], type, refID);
        }
    }
    catch (const std::exception& e)
    {
        throw;
    }
}

void LoadingJson::ID::ConvertComponentIDs(nlohmann::json& component, Scene::ComponentType type, std::vector<SPrefabHierarchy>& listOfHierarchy)
{
    const unsigned prefabID = component[Field::prefabIdField].get<unsigned>();

    component[Field::idField] = ID::CreateTopLevelID(
        component[Field::idField].get<unsigned>(),
        type,
        prefabID);

    ConvertRefFieldIDs(component[Field::inputsField][Field::gameObjectsField], Scene::ComponentType::gameObj, listOfHierarchy, prefabID);
    ConvertRefFieldIDs(component[Field::inputsField][Field::scriptsField], Scene::ComponentType::script, listOfHierarchy, prefabID);
}

void LoadingJson::ID::ConvertIDInJson(nlohmann::json& out, SPrefabHierarchy hierarchy)
{
    try
    {
        std::vector<SPrefabHierarchy> listHierarchy;
        PrefabHierarchy::ConstructVectorOfHierarchy(listHierarchy, hierarchy);
        for (auto & gameObj : out[Field::gameObjectsField])
            ConvertComponentIDs(gameObj, Scene::ComponentType::gameObj, listHierarchy);
        for (auto& script : out[Field::scriptsField])
            ConvertComponentIDs(script, Scene::ComponentType::script, listHierarchy);
    }
    catch (const std::exception& e)
    {
        std::wostringstream os;
        os << L"[Exception] the json object is in a wrong format, exception throw when converting id\n";
        os << e.what() << std::endl;
        std::ostringstream os2;
        os2 << out;
        os << L"[Error json object] " << os2.str().c_str() << std::endl;
        throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
    }
}
#pragma endregion

Scene::ComponentType LoadingJson::ID::ConvertStrToType(const std::string& str)
{
    const auto rValue = _typeDict.find(str);
    if (rValue == _typeDict.end())
        return Scene::ComponentType::invalid;
    return rValue->second;
}