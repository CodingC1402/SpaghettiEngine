#include "Body2D.h"
#include "Shape.h"

SBody2D Body2D::_defaultBody = std::make_shared<Body2D>();

WBody2D Body2D::GetDefaultBody()
{
	return _defaultBody;
}

void Body2D::AddShape(const SShape& shape)
{
	if (shape->GetBody().lock().get() == this)
		return;
	_shapes.push_back(shape);
}

void Body2D::RemoveShape(const SShape& shape)
{
	_shapes.remove(shape);
}

void Body2D::SetOrient(float radians)
{
	_orient = radians;
	for (auto& shape : _shapes)
		shape->SetOrient(radians);
}

void Body2D::SetStatic()
{
}

void Body2D::ApplyImpulse(const Vector3& impulse, const Vector3& contactVector)
{
}

void Body2D::ApplyForce(const Vector3& force)
{
}