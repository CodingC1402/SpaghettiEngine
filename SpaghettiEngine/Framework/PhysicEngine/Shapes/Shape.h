#pragma once
#include "Body2D.h"
#include "Matrix.h"
#include "CollideEvent.h"
#include <unordered_map>

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

	void SetOwnerScript(Collider2DBase* owner);
	[[nodiscard]] Collider2DBase* GetOwnerScript();

	[[nodiscard]] float GetInverseMass() const;
	[[nodiscard]] float GetMass() const;
	[[nodiscard]] Vector3 GetVelocity() const;
	[[nodiscard]] Vector3 GetGravityVector() const;
	[[nodiscard]] const Vector3& GetCenter() const;
	
	void SetOffSetX(const float& x);
	void SetOffSetY(const float& y);
	[[nodiscard]] float GetOffSetX() const;
	[[nodiscard]] float GetOffSetY() const;

	void SetTriggerOnly(bool value);
	[[nodiscard]] bool IsTriggerOnly() const;

	void SendEvent(CollideEvent& e);
	virtual void UpdateParameter() = 0;

	void RemoveFromPhysic();
	void AddToPhysic();

	void SetBody(WBody2D body);
	
	virtual Shape* Clone() const = 0;
	WBody2D GetBody() const;
protected:
	Collider2DBase* _ownerScript;

	WBody2D _body;
	bool _isTriggerOnly = false;

	Matrix4 _offSetMatrix;
	Matrix4 _worldMatrix;
	bool _isStatic = true;

	//AABB broad phase
	Vector3 _topLeft;
	Vector3 _bottomRight;
	Vector3 _center;

	Vector3 _worldTopLeft;
	Vector3 _worldBottomRight;
	Vector3 _worldCenter;
};

class ShapeFactory
{
public:
	static Shape* Create(const std::string& type);
protected:
	template<typename T>
	static Shape* CreateNew();
	static std::unordered_map<const char*, Shape*(*)()> _functions;
};

template<typename T>
inline Shape* ShapeFactory::CreateNew()
{
	return new T;
}
