#pragma once 

#include "Corntainer.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(GameObj);

class ChildContainer : public Corntainer<PGameObj>
{
public:
	ChildContainer(PGameObj owner);

	[[nodiscard]] PGameObj				GetItem(const std::string& name);
	[[nodiscard]] PGameObj				GetItemWithTag(const std::string& tag);
	[[nodiscard]] std::deque<PGameObj>	GetAllItemsWithName(const std::string& name);
	[[nodiscard]] std::deque<PGameObj>	GetAllItemsWithTag(const std::string& tag);

	SGameObj	AddItemClone(PGameObj child);
	void		AddItem(PGameObj child) override;
	
	void RemoveAllItem() override; 
	void RemoveItem(PGameObj object) override;
	void RemoveItemsWithName(const std::string& name);
	void RemoveItemsWithTag(const std::string& tag);
protected:
	// Used to do all the operation needed when remove child
	void RemoveChild(PGameObj object);
protected:
	PGameObj _owner;
};