#pragma once
#include <memory>
#include "ResourceContainer.h"

class Material;
typedef std::shared_ptr<Material> SMaterial;
typedef std::weak_ptr<Material> WMaterial;

class Material : public Resource
{
public:
	float _staticFriction;
	float _dynamicFriction;
	float _restitution;
};

class MaterialContainer : public Container<Material>
{
public:
	MaterialContainer();
};