#pragma once
#include <memory>
#include <map>

class Material;
typedef std::shared_ptr<Material> SMaterial;
typedef std::weak_ptr<Material> WMaterial;

class Material
{
	friend class Shape;
public:
	Material(const float staticFriction, const float dynamicFriction, const float restitution);
	static WMaterial CreateMaterial(const unsigned id, const float staticFriction, const float dynamicFriction, const float restitution);
	static WMaterial GetMaterial(const unsigned id);

	[[nodiscard]] float GetStaticFriction() const noexcept;
	[[nodiscard]] float GetDynamicFriction() const noexcept;
	[[nodiscard]] float GetRestitution() const noexcept;

	void SetStaticFriction(const float f) noexcept;
	void SetDynamicFriction(const float f) noexcept;
	void SetRestitution(const float f) noexcept;
protected:
	float _staticFriction;
	float _dynamicFriction;
	float _restitution;
protected:
	static inline std::map<unsigned, SMaterial> _materials;
	static inline SMaterial _defaultMaterial = std::make_shared<Material>(1.5, 1, 0);
};