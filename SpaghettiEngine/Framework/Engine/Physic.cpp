#include "Physic.h"

PPhysic Physic::__instance = NULL;

void Physic::Init()
{
}

void Physic::Update()
{
	for (const auto& rigidbody : renderBuffer)
	{
		rigidbody->Update();
	}
}

Physic::~Physic()
{
}

Physic* Physic::GetInstance()
{
	if (!__instance)
		__instance = new Physic();
	return __instance;
}
