#pragma once
#include <memory>
#include <list>
#include "Vector3.h"
#include "Material.h"
#include "CollideEvent.h"

class Body2D;
typedef std::weak_ptr<Body2D> WBody2D;
typedef std::shared_ptr<Body2D> SBody2D;

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

class Collider2DBase;
typedef std::weak_ptr<Collider2DBase> WCollider2DBase;

class Body2D 
{
	friend class Shape;
public:
	static WBody2D GetDefaultBody();

	void AddShape(Shape* shape);
	void RemoveShape(Shape* shape);

	void SetMass(const float& mass);
	[[nodiscard]] const float& GetMass() const;
	[[nodiscard]] const float& GetInverseMass() const;

	void SetVelocity(const Vector3& velocity);
	[[nodiscard]] const Vector3& GetVelocity() const;

	void SetForce(const Vector3& force);
	[[nodiscard]] const Vector3& GetForce() const;

	void SetPosition(const Vector3& pos);
	[[nodiscard]] const Vector3 GetPosition() const;

	void SetWorldMatrix(const Matrix4& mat);
	[[nodiscard]] const Matrix4& GetWorldMatrix() const;

	void SetRotation(const float& degree);
	[[nodiscard]] const float& GetRoation();

	void SetGravityScale(const float& scale);
	[[nodiscard]] const float& GetGravityScale() const;

	void SetGameObject(GameObj* collider);
	[[nodiscard]] GameObj* GetGameObject() const;

	void SendEvent(CollideEvent& e);
	void SendExitEnterEvent();

	[[nodiscard]] Vector3 GetMoveVector();
	[[nodiscard]] float GetRotation();

	void SetMaterial(WMaterial material);
	[[nodiscard]] WMaterial GetMaterial() const;

	void SetMaterialToDefault();
	void SetStatic();

	void ApplyImpulse(const Vector3& impulse);
	void ApplyForce(const Vector3& force);

	void IntergateForces();
	void IntergateVelocity();

	virtual void OnUpdateMatrix();
protected:
	static SBody2D _defaultBody;

	GameObj* _gameObject;

	Matrix4 _worldMatrix; 
	Vector3 _velocity;

	Vector3 _force;
	float _rotation = 0;

	float _inertia = 0;
	float _inverseInertia = 0;

	float _mass = 0;
	float _inverseMass = 0;

	float _gravityScale = 1;

	Vector3 _moveVec;

	std::list<Shape*> _shapes;
	std::list<WBody2D> _collidedBody;
	std::list<WBody2D> _currentCollide;
	mutable SMaterial _material = Material::GetDefaultMaterial().lock();
};