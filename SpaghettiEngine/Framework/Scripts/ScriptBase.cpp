#include "ScriptBase.h"

ScriptTypes* ScriptFactory::map = nullptr;


ScriptTypes* ScriptFactory::GetMap()
{
	if (!map)
		map = new ScriptTypes();
	return map;
}

ScriptBase* ScriptFactory::CreateInstance(std::string const& typeName)
{
	auto iterator = map->find(typeName);
	if (iterator == GetMap()->end())
		return nullptr;
	return iterator->second();
}