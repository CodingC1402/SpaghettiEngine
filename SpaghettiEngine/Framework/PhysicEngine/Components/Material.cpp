#include "Material.h"

Material::Material(const float staticFriction, const float dynamicFriction, const float restitution)
	:
	_staticFriction(staticFriction),
	_dynamicFriction(dynamicFriction),
	_restitution(restitution)
{}

WMaterial Material::CreateMaterial(const unsigned id, const float staticFriction, const float dynamicFriction, const float restitution)
{
	SMaterial newMaterial = std::make_shared<Material>(staticFriction, dynamicFriction, restitution);
	_materials.emplace(id, newMaterial);
	return newMaterial;
}

WMaterial Material::GetMaterial(const unsigned id)
{
	return _materials[id];
}

float Material::GetStaticFriction() const noexcept
{
	return _staticFriction;
}

float Material::GetDynamicFriction() const noexcept
{
	return _dynamicFriction;
}

float Material::GetRestitution() const noexcept
{
	return _restitution;
}

void Material::SetStaticFriction(const float f) noexcept
{
	_staticFriction = f;
}

void Material::SetDynamicFriction(const float f) noexcept
{
	_dynamicFriction = f;
}

void Material::SetRestitution(const float f) noexcept
{
	_restitution = f;
}
