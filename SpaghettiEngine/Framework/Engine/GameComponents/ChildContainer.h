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

	SGameObj	AddItemClone(PGameObj child);
	void		AddItem(PGameObj child) override;
	
	void RemoveAllItem() override; 
	void RemoveItem(PGameObj object) override;
	void RemoveItemsWithName(const std::string& name);
	void RemoveItemsWithTag(const std::string& tag);
protected:
	// Used to do all the operation needed when remove child
	void RemoveChild(PGameObj object);
	void RemoveChildWithoutEvent(PGameObj object);
protected:
	PGameObj _owner;
};