#pragma once
#include "Body2D.h"
#include "Matrix.h"
#include "CollideEvent.h"

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

class Collider2DBase;
typedef std::weak_ptr<Collider2DBase> WCollider2DBase;

class Collision;

class Shape
{
public:
	enum class Type
	{
		Circle,
		Polygon,
		TypeCount,
		Invalid
	};
public:
	Shape();
	virtual ~Shape();

	[[nodiscard]] virtual Type GetType() const;

	void SetOwnerScript(WCollider2DBase owner);
	[[nodiscard]] WCollider2DBase GetOwnerScript();

	[[nodiscard]] float GetInverseMass() const;
	[[nodiscard]] float GetMass() const;
	[[nodiscard]] Vector3 GetVelocity() const;
	[[nodiscard]] Vector3 GetGravityVector() const;
	
	void SetOffSetX(const float& x);
	void SetOffSetY(const float& y);
	[[nodiscard]] float GetOffSetX() const;
	[[nodiscard]] float GetOffSetY() const;

	[[nodiscard]] const Vector3& GetCenter() const;
	[[nodiscard]] const float& GetRadius() const;

	void SendEvent(CollideEvent& e);

	virtual void UpdateParameter() = 0;

	void RemoveFromPhysic();
	void AddToPhysic();

	void SetBody(WBody2D body);
	
	virtual Shape* Clone() const = 0;
	WBody2D GetBody() const;
protected:
	WCollider2DBase _ownerScript;

	WBody2D _body;

	Matrix4 _offSetMatrix;
	Matrix4 _worldMatrix;
	bool _isStatic = true;

	//Use for broad phase
	Vector3 _center;
	float _radius;
};