#pragma once
#include "Body2D.h"
#include "Matrix.h"

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

class Collision;

class Shape
{
public:
	enum class Type
	{
		Circle,
		Polygon,
		TypeCount
	};
public:
	Shape();
	virtual Type GetType() const = 0;

	[[nodiscard]] float GetInverseMass();
	[[nodiscard]] float GetMass();
	[[nodiscard]] Vector3 GetVelocity();
	[[nodiscard]] Vector3 GetGravityVector();
	
	void SetOffSetX(const float& x);
	void SetOffSetY(const float& y);
	[[nodiscard]] const float& GetOffSetX();
	[[nodiscard]] const float& GetOffSetY();

	[[nodiscard]] Vector3& GetCenter();
	[[nodiscard]] float& GetRadius();

	virtual void UpdateParameter() = 0;

	void RemoveFromPhysic();
	void AddToPhysic();

	void SetBody(WBody2D body);
	void ToStatic();

	WBody2D GetBody();
protected:
	WBody2D _body;
	SBody2D _staticBody;

	Matrix4 _offSetMatrix;
	Matrix4 _worldMatrix;
	bool _isStatic = true;

	//Use for broad phase
	Vector3 _center;
	float _radius;
};