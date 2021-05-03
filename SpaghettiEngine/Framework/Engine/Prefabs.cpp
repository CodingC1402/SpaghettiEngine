#include "Prefabs.h"
#include "SpaghettiEnginePath.h"
#include "ExMath.h"
#include "CornException.h"
#include "Setting.h"
#include <ranges>

CONTAINER_REGISTER(PrefabsContainer, Prefab);

constexpr const char* id = "ID";
constexpr const char* gameObjects = "GameObjects";
constexpr const char* scripts = "Scripts";

Prefab::ComponentJsonObject::ComponentJsonObject(nlohmann::json& jsonObject, const bool& isGameObject)
	:
	_jsonObject(jsonObject),
	_isGameObject(isGameObject)
{}

void Prefab::Append(nlohmann::json& out, unsigned int index, nlohmann::json& changes)
{
	using nlohmann::json;

	if constexpr  (Setting::IsDebugMode())
	{
		if (index == 0)
			throw RESOURCE_LOAD_EXCEPTION("[Exception] index of prefab is 0", Prefab);
		if (index & Setting::_errorMaskPrefabIndex)
			throw RESOURCE_LOAD_EXCEPTION("[Exception] index of prefab is langer then what defined in setting", Prefab);
	}
	
	auto appendCopy = _components;
	try
	{	
		if (changes != nullptr)
		{	
			for (unsigned int affectedId; const auto & change : changes)
			{
				constexpr const char* field = "Field";
				constexpr const char* value = "Value";
				
				affectedId = change[id].get<unsigned int>();
				const auto it = appendCopy.find(affectedId);
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
				it->second._jsonObject[change[field].get<std::string>()] = change[value];
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
		constexpr auto scriptsField = "Scripts";
		constexpr auto childrenField = "Children";
		
		for (auto& val : appendCopy | std::views::values)
		{
			constexpr auto prefabIDField = "PrefabID";
			
			val._jsonObject[prefabIDField] = index;
			if (val._isGameObject)
				out[gameObjects].emplace_back(val._jsonObject);
			else
				out[scripts].emplace_back(val._jsonObject);
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

		for (auto& object : loadedJson[gameObjects])
		{
			if constexpr (Setting::IsDebugMode())
			{
				const auto currentId = object[id].get<unsigned int>();
				if ((currentId & Setting::_errorMaskLocalId) > 0)
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
				_components.emplace(object[id].get<unsigned int>(), ComponentJsonObject(object, true));
			}
		}
		for (auto& script : loadedJson[scripts])
		{
			if constexpr(Setting::IsDebugMode())
			{
				const auto currentId = script[id].get<unsigned int>();
				if ((currentId & Setting::_errorMaskLocalId) > 0)
				{
					std::ostringstream os;
					os << "[Exception] The id of the game object is larger then 32 bit" << std::endl;
					os << "[Game object] " << script << std::endl;
					throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
				}

				if (const auto result = _components.emplace(script[id].get<unsigned int>(), ComponentJsonObject(script, false)); !result.second)
				{
					if (result.first->second._isGameObject)
					{
						std::ostringstream os;
						os << "[Exception] There is a game objects with the same id as the script" << std::endl;
						os << "[Game object] " << result.first->second._jsonObject << std::endl;
						os << "[Script     ] " << script << std::endl;
						throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
					}
					else
					{
						std::ostringstream os;
						os << "[Exception] There is a game objects with the same id as the script" << std::endl;
						os << "[Game object] " << result.first->second._jsonObject << std::endl;
						os << "[Script     ] " << script << std::endl;
						throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
					}
				}
			}
			else
			{
				_components.emplace(script[id].get<unsigned int>(), ComponentJsonObject(script, false));
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
