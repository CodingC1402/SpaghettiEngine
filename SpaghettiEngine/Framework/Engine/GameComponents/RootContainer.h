#pragma once
#include "Corntainer.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(GameObj);

class RootContainer : public Corntainer<PGameObj>
{
public:
	void AddItem(PGameObj item) override;
	void RemoveItem(PGameObj item) override;

	void RemoveAllItem() override;
};