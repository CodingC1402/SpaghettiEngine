#pragma once
#include "ResourceContainer.h"
#include "Material.h"

class MaterialContainer : public Container<Material>
{
	friend class SceneManager;
public:
	MaterialContainer();
};