#include "Prefabs.h"
#include "SpaghettiEnginePath.h"
#include "LoadingJson.h"
#include "CornException.h"
#include "Setting.h"
#include "Scene.h"
#include <ranges>

using namespace LoadingJson;
CONTAINER_REGISTER(PrefabsContainer, Prefab);

Prefab::ComponentJsonObject::ComponentJsonObject(nlohmann::json& jsonObject, const bool& isGameObject)
	:
	_jsonObject(jsonObject),
	_isGameObject(isGameObject)
{}

void Prefab::Append(nlohmann::json& out, unsigned int index, nlohmann::json& changes)
{
	using nlohmann::json;
	using namespace LoadingJson;

	if constexpr  (Setting::IsDebugMode())
	{
		if (!ID::CheckPrefabIndex(index))
			throw RESOURCE_LOAD_EXCEPTION("[Exception] index of prefab is langer then what defined in id or equal to zero", Prefab);
	}
	
	auto appendCopy = _components;
	try
	{	
		if (changes != nullptr)
		{	
			for (ULL affectedId; const auto & change : changes)
			{
				affectedId = ID::CreateLocalLevelID(change[idField], ID::ConvertStrToType(change[typeField].get<std::string>()));
				
				const auto& it = appendCopy.find(affectedId);
				if constexpr (Setting::IsDebugMode())
				{
					if (it == appendCopy.end())
					{
						std::ostringstream os;
						os << "[Exception] Component with ID " << affectedId << "doesn't exist in the requested prefab to make change" << std::endl;
						os << "[Change object] " << change << std::endl;
						throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
					}
				}
				it->second._jsonObject[inputsField][change[fieldField].get<std::string>()] = change[valueField];
			}
		}
	}
	catch (const CornException&)
	{
		throw;
	}
	catch (const std::exception& e)
	{
		std::ostringstream os;
		os << "[Exception] Change object is in the wrong format" << std::endl;
		os << "[Change object exception] " << e.what() << std::endl;
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
	}
	
	try
	{
		for (auto& val : appendCopy | std::views::values)
		{
			val._jsonObject[prefabIdField] = index;
			if (val._isGameObject)
				out[gameObjectsField].emplace_back(val._jsonObject);
			else
				out[scriptsField].emplace_back(val._jsonObject);
		}
	}
	catch(const std::exception& e)
	{
		std::ostringstream os;
		os << "[Exception] Error while appending the component to the json file" << std::endl;
		os << "[Standard exception] " << e.what() << std::endl;
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
	}
}

void Prefab::Load(const std::string& path)
{
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Animation);
	}

	try
	{	
		json loadedJson;
		file >> loadedJson;
		
		for (auto& object : loadedJson[gameObjectsField])
		{
			const auto currentId = ID::CreateLocalLevelID(object[idField].get<unsigned int>(), Scene::ComponentType::gameObj);
			if constexpr (Setting::IsDebugMode())
			{
				if (!ID::CheckID(object[idField].get<unsigned int>()))
				{
					std::ostringstream os;
					os << "[Exception] The id of the game object is larger then 32 bit" << std::endl;
					os << "[Game object] " << object << std::endl;
					throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
				}

				if (const auto result = _components.emplace(currentId, ComponentJsonObject(object, true)); !result.second)
				{
					std::ostringstream os;
					os << "[Exception] There are 2 game objects with the same id" << std::endl;
					os << "[First object ] " << result.first->second._jsonObject << std::endl;
					os << "[Second object] " << object << std::endl;
					throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
				}
			}
			else
			{
				_components.emplace(currentId, ComponentJsonObject(object, true));
			}
		}
		for (auto& script : loadedJson[scriptsField])
		{
			const auto currentId = ID::CreateLocalLevelID(script[idField].get<unsigned int>(), Scene::ComponentType::script);
			if constexpr(Setting::IsDebugMode())
			{
				if (!ID::CheckID(script[idField].get<unsigned int>()))
				{
					std::ostringstream os;
					os << "[Exception] The id of the game object is larger then 32 bit" << std::endl;
					os << "[Game object] " << script << std::endl;
					throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
				}

				if (const auto result = _components.emplace(currentId, ComponentJsonObject(script, false)); !result.second)
				{
					if (result.first->second._isGameObject)
					{
						std::ostringstream os;
						os << "[Exception] There is a game objects with the same id as the script" << std::endl;
						os << "[Game object] " << result.first->second._jsonObject << std::endl;
						os << "[Script] " << script << std::endl;
						throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
					}
					else
					{
						std::ostringstream os;
						os << "[Exception] There are 2 scripts with the same id" << std::endl;
						os << "[Script1] " << result.first->second._jsonObject << std::endl;
						os << "[Script2] " << script << std::endl;
						throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
					}
				}
			}
			else
			{
				_components.emplace(currentId, ComponentJsonObject(script, false));
			}
		}
	}
	catch (const CornException&)
	{
		throw;
	}
	catch (const std::exception& e)
	{
		std::ostringstream os;
		os << "[Exception]" << e.what() << std::endl;
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
	}
}

PrefabsContainer::PrefabsContainer()
{
	_name = RESOURCE_NAME(Prefab);
	LoadEntries(SystemPath::PrefabEntriesPath);
}
