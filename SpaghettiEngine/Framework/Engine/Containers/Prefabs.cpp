#include "Prefabs.h"
#include "SpaghettiEnginePath.h"
#include "LoadingJson.h"
#include "CornException.h"
#include "Setting.h"
#include "Scene.h"
#include <ranges>

using namespace LoadingJson;
CONTAINER_REGISTER(PrefabsContainer, Prefab);

void PrefabHierarchy::ConstructVectorOfHierarchy(std::vector<SPrefabHierarchy>& out, SPrefabHierarchy root)
{
	out.push_back(root);
	root->ConstructVectorOfChildHierarchy(out);
}

PrefabHierarchy::PrefabHierarchy(int numberOfChild, int value)
{
	_children.reserve(numberOfChild);
	_value = value;
}

void PrefabHierarchy::AddChild(std::shared_ptr<PrefabHierarchy>& child)
{
	_children.emplace_back(child);
}

unsigned PrefabHierarchy::GetIndex(std::vector<unsigned>& accessIndexes)
{
	return GetIndexRecursive(accessIndexes, 0);
}

void PrefabHierarchy::ConstructVectorOfChildHierarchy(std::vector<SPrefabHierarchy>& out)
{
	for (const auto& child : _children)
	{
		out.push_back(child);
		child->ConstructVectorOfChildHierarchy(out);
	}
}

unsigned PrefabHierarchy::GetIndexRecursive(std::vector<unsigned>& accessIndexes, unsigned level)
{
	try
	{
		if (level >= accessIndexes.size())
			return _value;
		return _children[accessIndexes[level]]->GetIndexRecursive(accessIndexes, ++level);
	}
	catch(const std::exception& e)
	{	
		std::ostringstream os;
		os << "[Access indexed] ";
		for (unsigned i = 0; i < accessIndexes.size(); i++)
			os << accessIndexes[i] << ", ";
		os << std::endl;
		os << "[Number of level] " << accessIndexes.size() << std::endl;
		os << "[Level] " << level << std::endl;
		os << "[Exception] There was an error while trying to get the index in prefab hierarchy";

		throw CONTAINER_EXCEPT(PrefabsContainer::GetName(), os.str());
	}
}

Prefab::ComponentJsonObject::ComponentJsonObject(nlohmann::json& jsonObject, const bool& isGameObject)
	:
	_jsonObject(jsonObject),
	_isGameObject(isGameObject)
{}

void ApplyIndexToPrefabsChanges(nlohmann::json& out, unsigned int index)
{
	for(auto& prefab : out)
		for (auto& change : prefab[Field::changesField])
			if (Field::IsRefField(change[Field::fieldField].get<std::string>()))
				for (auto& ref : change[Field::valueField])
					if (!ref.empty())
						ref[Field::prefabIdField] = index;
}

NLOHMANN_JSON_SERIALIZE_ENUM(Prefab::ChangeMode, {
	{Prefab::ChangeMode::Update, "Update"},
	{Prefab::ChangeMode::Truncate, "Truncate"},
	{Prefab::ChangeMode::Append, "Append"}
	})
void ApplyChangeToInputField(nlohmann::json& input, nlohmann::json& change)
{
	if (const auto field = change[Field::fieldField].get<std::string>(); Field::IsRefField(field))
	{
		if (change[Field::modeField].empty())
			change[Field::modeField] = "Update";

		switch (change[Field::modeField].get<Prefab::ChangeMode>())
		{
		case Prefab::ChangeMode::Append:
			for (auto& ref : change[Field::valueField])
				if (!ref.empty())
					input[field].emplace_back(ref);
			break;
		case Prefab::ChangeMode::Truncate:
			input[field].clear();
			[[fallthough]]
		case Prefab::ChangeMode::Update:
			for (auto& ref : change[Field::valueField])
				if (!ref.empty())
					input[field][ref[Field::refIndex].get<unsigned>()] = ref;
			break;
		}
	}
	else
		input[field] = change[Field::valueField];
}

SPrefabHierarchy Prefab::Append(nlohmann::json& out, unsigned int& index, nlohmann::json& changes)
{
	using nlohmann::json;
	using namespace LoadingJson;
	
	if constexpr  (Setting::IsDebugMode())
	{
		if (!ID::CheckPrefabIndex(index))
		{
			std::ostringstream os;
			os << "[Exception] You are using more than 2 ^ ";
			os << ID::_bitForPrefabId << " - 1 total prefabs(including sub prefabs) in one level... wth dude :v";
			throw RESOURCE_LOAD_EXCEPTION(os.str(), Prefab);
		}
	}
	
	auto appendCopy = _components;
	auto subPrefabsCopy = _subPrefabs;
	
	try // Apply changes
	{
		ApplyIndexToPrefabsChanges(subPrefabsCopy, index);
		
		if (changes != nullptr) // Apply changes and forward sub changes to sub prefabs
		{
			bool isSubChange = false;
			for (ULL affectedId; auto & change : changes)
			{
				isSubChange = false;
				
				if (!change[Field::prefabsField].empty())
				{
					if (change[Field::levelField].empty())
						change[Field::levelField] = 0;

					if (unsigned level; change[Field::levelField].get<unsigned>() < change[Field::prefabIdField].size())
					{
						level = change[Field::levelField].get<unsigned>();
						change[Field::levelField] = level + 1;
						subPrefabsCopy[change[Field::prefabsField][level].get<unsigned>()][Field::changesField].emplace_back(change);
						isSubChange = true;
					}
				}

				if (!isSubChange)
				{
					affectedId = ID::CreateLocalLevelID(change[Field::idField], ID::ConvertStrToType(change[Field::typeField].get<std::string>()));

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
					ApplyChangeToInputField(it->second._jsonObject[Field::inputsField], change);
				}
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
			val._jsonObject[Field::prefabIdField] = index;
			if (val._isGameObject)
				out[Field::gameObjectsField].emplace_back(val._jsonObject);
			else
				out[Field::scriptsField].emplace_back(val._jsonObject);
		}

		SPrefabHierarchy prefabHierarchy = std::make_shared<PrefabHierarchy>(_subPrefabsIDs.size(), index);
		index++; // Increase index
		for (SPrefabHierarchy subHierarchy; auto& subPrefab : subPrefabsCopy)
		{
			subHierarchy = PrefabsContainer::GetInstance()->GetResource(subPrefab[Field::idField].get<unsigned>())->Append(out, index, subPrefab[Field::changesField]);
			prefabHierarchy->AddChild(subHierarchy);
		}
		return prefabHierarchy;
	}
	catch (const CornException& e)
	{
		throw;
	}
	catch(const std::exception& e)
	{
		std::ostringstream os;
		os << "[Exception] Error while appending prefabs component to the json file" << std::endl;
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
		
		if (loadedJson[Field::prefabsField] != nullptr)
		{
			_subPrefabs = loadedJson[Field::prefabsField];
			for(const auto& prefab : loadedJson[Field::prefabsField])
				_subPrefabsIDs.emplace_back(prefab[Field::idField].get<CULL>());
		}
		
		for (auto& object : loadedJson[Field::gameObjectsField])
		{
			const auto currentId = ID::CreateLocalLevelID(object[Field::idField].get<unsigned int>(), Scene::ComponentType::gameObj);
			if constexpr (Setting::IsDebugMode())
			{
				if (!ID::CheckID(object[Field::idField].get<unsigned int>()))
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
		for (auto& script : loadedJson[Field::scriptsField])
		{
			const auto currentId = ID::CreateLocalLevelID(script[Field::idField].get<unsigned int>(), Scene::ComponentType::script);
			if constexpr(Setting::IsDebugMode())
			{
				if (!ID::CheckID(script[Field::idField].get<unsigned int>()))
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
