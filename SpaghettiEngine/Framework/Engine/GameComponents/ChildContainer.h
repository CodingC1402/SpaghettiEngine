#pragma once 

#include "Corntainer.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(GameObj);

class ChildContainer : public Corntainer<PGameObj>
{
	friend class GameObj;
public:
	ChildContainer(PGameObj owner);

	[[nodiscard]] PGameObj				GetItem(const std::string& name);
	[[nodiscard]] PGameObj				GetItemWithTag(const std::string& tag);
	[[nodiscard]] std::list<PGameObj>	GetAllItemsWithName(const std::string& name);
	[[nodiscard]] std::list<PGameObj>	GetAllItemsWithTag(const std::string& tag);

	PGameObj	AddItemClone(PGameObj child);
	void		AddItem(PGameObj child) override;
	
	void RemoveAllItem() override; 
	void RemoveItem(PGameObj object) override;
	void RemoveItemsWithName(const std::string& name);
	void RemoveItemsWithTag(const std::string& tag);

	void DisableAll();
	void EnableAll();
protected:
	// Call of the thing that needed so that other function can safely erase it from container.
	void RemoveChild(PGameObj object);
	// Use to do thing that doesn't involve event before delete
	void RemoveChildFromTransform(PGameObj object);
	// Use to remove child from the list and to all the stuff needed without moving it or call events
	void RemoveChildWithoutEvent(PGameObj object);
protected:
	PGameObj _owner;
};