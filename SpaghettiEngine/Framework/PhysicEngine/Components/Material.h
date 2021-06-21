#pragma once
#include <map>
#include <memory>
#include "ResourceContainer.h"

class Material;
typedef std::shared_ptr<Material> SMaterial;
typedef std::weak_ptr<Material> WMaterial;

class Material : public Resource
{
public:
	Material();
	Material(const float staticFriction, const float dynamicFriction, const float restitution);
	[[nodiscard]] static WMaterial GetDefaultMaterial();

	[[nodiscard]] const float& GetStaticFriction() const noexcept;
	[[nodiscard]] const float& GetDynamicFriction() const noexcept;
	[[nodiscard]] const float& GetRestitution() const noexcept;

	void Load(const std::string& path) override;

	void SetStaticFriction(const float f) noexcept;
	void SetDynamicFriction(const float f) noexcept;
	void SetRestitution(const float f) noexcept;
protected:
	float _staticFriction;
	float _dynamicFriction;
	float _restitution;
protected:
	static inline SMaterial _defaultMaterial = std::make_shared<Material>(0.1f, 0.1f, 0.0f);
};