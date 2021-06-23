#include "LoadingJson.h"
#include "CornException.h"
#include "Prefabs.h"

std::list<std::string> LoadingJson::Field::_refFields{
    gameObjectsField,
    scriptsField
};

std::unordered_map<std::string, BaseComponent::Type> LoadingJson::ID::_typeDict{
    {"GameObject", BaseComponent::Type::gameObj},
    {"Script", BaseComponent::Type::script}
};

bool LoadingJson::Field::IsRefField(const std::string& field)
{
    for (const auto& refField : _refFields)
        if (refField == field)
            return true;
    return false;
}

#pragma region Convert IDs
void LoadingJson::ID::ConvertRefFieldIDs(nlohmann::json& refField, BaseComponent::Type type, std::vector<SPrefabHierarchy>& listOfHierarchy, const unsigned& defaultID)
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
    catch (const std::exception&)
    {
        throw;
    }
}

void LoadingJson::ID::ConvertComponentIDs(nlohmann::json& component, BaseComponent::Type type, std::vector<SPrefabHierarchy>& listOfHierarchy)
{
    const unsigned prefabID = component[Field::prefabIdField].get<unsigned>();

    component[Field::idField] = ID::CreateTopLevelID(
        component[Field::idField].get<unsigned>(),
        type,
        prefabID);

    ConvertRefFieldIDs(component[Field::inputsField][Field::gameObjectsField], BaseComponent::Type::gameObj, listOfHierarchy, prefabID);
    ConvertRefFieldIDs(component[Field::inputsField][Field::scriptsField], BaseComponent::Type::script, listOfHierarchy, prefabID);
}

void LoadingJson::ID::ConvertIDInJson(nlohmann::json& out, SPrefabHierarchy hierarchy)
{
    try
    {
        std::vector<SPrefabHierarchy> listHierarchy;
        PrefabHierarchy::ConstructVectorOfHierarchy(listHierarchy, hierarchy);
        for (auto & gameObj : out[Field::gameObjectsField])
            ConvertComponentIDs(gameObj, BaseComponent::Type::gameObj, listHierarchy);
        for (auto& script : out[Field::scriptsField])
            ConvertComponentIDs(script, BaseComponent::Type::script, listHierarchy);
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

BaseComponent::Type LoadingJson::ID::ConvertStrToType(const std::string& str)
{
    const auto rValue = _typeDict.find(str);
    if (rValue == _typeDict.end())
        return BaseComponent::Type::invalid;
    return rValue->second;
}