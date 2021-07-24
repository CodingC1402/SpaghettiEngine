#pragma once
#include "RidableScript.h"

// Use this for enemies and stars
class ObjectRidable : public RidableScript
{
public:
	void OnFixedUpdate() override;
	void OnCollide(CollideEvent& e) override;
	void OnDisabled() override;
private:
	static inline ObjectRidable* __instance;

	REGISTER_START(ObjectRidable);
};

