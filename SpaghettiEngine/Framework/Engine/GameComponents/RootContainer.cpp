#include "RootContainer.h"
#include "GameObj.h"

void RootContainer::AddItem(PGameObj item)
{
	if (!item)
		return;

	Add(item);
	item->SetContainerIterator(--_container.end());
	item->SetIsRoot(true);
}

void RootContainer::RemoveItem(PGameObj item)
{
	if (!item || !item->IsRoot())
		return;

	Erase(item->GetContainerIterator());
	item->SetIsRoot(false);
}

void RootContainer::RemoveAllItem()
{
	auto it = _container.begin();
	while (it != _container.end())
	{
		(*it)->SetIsRoot(false);
		it = Erase(it);
	}
}