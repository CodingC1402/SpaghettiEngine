#pragma once
#include "Body2D.h"
#include "Matrix.h"
#include "Macros.h"
#include "CollideEvent.h"
#include "BroadPhaseShape.h"

#include <unordered_map>

class Shape;
typedef std::weak_ptr<Shape> WShape;
typedef std::shared_ptr<Shape> SShape;

class Collider2DBase;
typedef std::weak_ptr<Collider2DBase> WCollider2DBase;

class Collision;

class Shape
{
	friend class ShapeFactory;
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
	[[nodiscard]] Collider2DBase* GetOwnerScript() const noexcept;

	[[nodiscard]] float GetInverseMass() const noexcept;
	[[nodiscard]] float GetMass() const noexcept;
	[[nodiscard]] Vector3 GetVelocity() const noexcept;
	[[nodiscard]] Vector3 GetGravityVector() const noexcept;
	[[nodiscard]] const Vector3& GetCenter() const noexcept;
	[[nodiscard]] const BroadPhaseShape& GetBroadPhase() const noexcept;
	
	void SetOffSetX(const float& x);
	void SetOffSetY(const float& y);
	[[nodiscard]] float GetOffSetX() const noexcept;
	[[nodiscard]] float GetOffSetY() const noexcept;

	void SetTriggerOnly(bool value);
	[[nodiscard]] bool IsTriggerOnly() const noexcept;

	void SendEvent(CollideEvent& e);

	// Return whether they updated or not
	virtual bool UpdateParameter();
	void UpdateParameter(const Matrix4& matrix);

	void RemoveFromPhysic();
	void AddToPhysic();

	void SetBody(WBody2D body);
	
	virtual Shape* Clone() const;
	[[nodiscard]] WBody2D GetBody() const noexcept;
protected:
	Collider2DBase* _ownerScript;

	WBody2D _body;
	bool _isTriggerOnly = false;

	Matrix4 _offSetMatrix;
	Matrix4 _worldMatrix;
	bool _isStatic = true;

	BroadPhaseShape _broadPhase;
};

class ShapeFactory
{
protected:
	class FactoryInitializer
	{
	public:
		FactoryInitializer();
	};
public:
	static Shape* Create(const std::string& type);
	static Shape* Create(const Shape::Type& type);
protected:
	template<typename T>
	static Shape* CreateNew();
protected:
	static inline std::unordered_map<std::string, Shape*(*)()> _functions;
	static inline FactoryInitializer _initializer;
};

template<typename T>
inline Shape* ShapeFactory::CreateNew()
{
	return new T;
}
