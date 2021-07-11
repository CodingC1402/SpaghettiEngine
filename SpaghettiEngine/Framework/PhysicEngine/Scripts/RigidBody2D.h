#pragma once
#include "PhysicScriptBase.h"
#include "Body2D.h"

typedef class RigidBody2D* PRigidBody2D;
typedef std::shared_ptr<RigidBody2D> SRigidBody2D;
typedef std::weak_ptr<RigidBody2D> WRigidBody2D;

class RigidBody2D : public PhysicScriptBase
{
	friend class PhysicComponent;
public:
	void SetVelocity(const Vector3& velocity);
	[[nodiscard]] const Vector3& GetVelocity() const;

	void SetMass(const float& mass);
	[[nodiscard]] const float& GetMass() const;

	void SetGravityScale(const float& scalar);
	[[nodiscard]] const float& GetGravityScale() const;

	void ApplyImpulseForce(const Vector3& force);

	void OnDisabled() override;
	void OnEnabled() override;

	void AfterPhysicUpdate();

	PScriptBase Clone() const override;
	void Load(nlohmann::json& input) override;
protected:
	[[nodiscard]] SBody2D GetBody();
protected:
	SBody2D _body;
private:
	REGISTER_START(RigidBody2D);
};