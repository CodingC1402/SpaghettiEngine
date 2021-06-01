#pragma once
#include <map>
#include <memory>

class Material;
typedef std::shared_ptr<Material> SMaterial;
typedef std::weak_ptr<Material> WMaterial;

class Material
{
public:
	Material(const float staticFriction, const float dynamicFriction, const float restitution);
	static WMaterial CreateMaterial(const unsigned id, const float staticFriction, const float dynamicFriction, const float restitution);
	[[nodiscard]] static WMaterial GetMaterial(const unsigned id);

	static bool RemoveMaterial(const unsigned id);
	[[nodiscard]] static WMaterial GetDefaultMaterial();

	[[nodiscard]] const float& GetStaticFriction() const noexcept;
	[[nodiscard]] const float& GetDynamicFriction() const noexcept;
	[[nodiscard]] const float& GetRestitution() const noexcept;

	void SetStaticFriction(const float f) noexcept;
	void SetDynamicFriction(const float f) noexcept;
	void SetRestitution(const float f) noexcept;
protected:
	float _staticFriction;
	float _dynamicFriction;
	float _restitution;
protected:
	static inline std::map<unsigned, SMaterial> _materials;
	static inline SMaterial _defaultMaterial = std::make_shared<Material>(0.4f, 0.2f, 0.0f);
};