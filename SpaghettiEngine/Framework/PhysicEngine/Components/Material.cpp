#include "Material.h"
#include "SpaghettiEnginePath.h"

CONTAINER_REGISTER(MaterialContainer, Material);

MaterialContainer::MaterialContainer()
{
	_name = RESOURCE_NAME(Material);
	LoadEntries(SystemPath::MaterialPath);
}
