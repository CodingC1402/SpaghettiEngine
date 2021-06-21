#include "MaterialContainer.h"
#include "SpaghettiEnginePath.h"

MaterialContainer::MaterialContainer()
{
	_name = RESOURCE_NAME(Material);
	LoadEntries(SystemPath::MaterialPath);
}
