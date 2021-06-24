#pragma once

#include "Corntainer.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(GameObj);
CLASS_FORWARD_DECLARATION(ScriptBase);

class ScriptContainer : public Corntainer<PScriptBase>
{
public:
	ScriptContainer(PGameObj owner);

	[[nodiscard]] PScriptBase				GetItem(const std::string& name);
	// Return the first script of the style
	[[nodiscard]] PScriptBase				GetItemType(const std::string& type);
	// Return all the script of the same style
	[[nodiscard]] std::list<PScriptBase>	GetAllItemsType(const std::string& type);

	PScriptBase	AddItemClone(PScriptBase script);
	void		AddItem(PScriptBase script) override;

	void RemoveAllItem() override;
	void RemoveItem(PScriptBase object) override;
	void RemoveItemsType(const std::string& type);
protected:
	// Used to do all the operation needed when remove child
	void RemoveScript(PScriptBase object);
protected:
	PGameObj _owner;
};