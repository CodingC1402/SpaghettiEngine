#pragma once
#include <memory>

class Material;
typedef std::shared_ptr<Material> SMaterial;
typedef std::weak_ptr<Material> WMaterial;

class Material
{
public:
};