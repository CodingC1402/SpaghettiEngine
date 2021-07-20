#pragma once
#include "RidableScript.h"

// Use this for enemies and stars
class ObjectRidable : public RidableScript
{
public:
	void OnCollide(CollideEvent& e) override;
private:
	REGISTER_START(ObjectRidable);
};

