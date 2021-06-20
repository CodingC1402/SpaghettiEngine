#include "Polygon.h"
#include "Collision.h"
#include "Circle.h"
#include "SMath.h"
#include <cmath>

Shape::Type Polygon::GetType() const
{
	return Type::Polygon;
}

bool Polygon::PolygonPolygon(Collision* collision)
{
	bool result = false;
	const auto shapeA = dynamic_cast<Polygon*>(collision->GetShapeA());
	const auto shapeB = dynamic_cast<Polygon*>(collision->GetShapeB());

	Vector3 normal;
	float penetration = 0;

	result = shapeA->CheckCollideOnEachEdge(*shapeB, penetration, normal);
	if (!result)
		return false;

	result = shapeB->CheckCollideOnEachEdge(*shapeA, penetration, normal);
	if (!result)
		return false;

	if (normal.Dot(shapeB->GetCenter() - shapeA->GetCenter()) < 0)
		normal = -normal;

	collision->SetPenetration(penetration);
	collision->SetNormal(normal);
	return result;
}

bool Polygon::PolygonCircle(Collision* collision)
{
	const auto polygon = dynamic_cast<Polygon2D*>(collision->GetShapeA());
	const auto circle = dynamic_cast<Circle*>(collision->GetShapeB());

	
	
	return false;
}

void Polygon::UpdateParameter()
{
	Matrix4 matrix = _body.lock()->GetWorldMatrix();
	if (matrix == _worldMatrix)
		return;

	_worldMatrix = matrix;
	_worldVertexes = _vertexes;
	for (auto& vertex : _worldVertexes)
		vertex = vertex * _offSetMatrix * _worldMatrix;
	_center = _centroid * _offSetMatrix * _worldMatrix;
}

bool Polygon::CheckCollideOnEachEdge(const Polygon& other, float& penetration, Vector3& normal) const
{
	const auto& vertexes = this->GetVertexes();

	const auto size = vertexes.size();

	Vector3 currentNormal;
	float currentPen;

	Vector3 edge = vertexes[0] - vertexes[1];
	if (!CheckCollideOnOneEdge(edge, other, currentPen, currentNormal))
		return false;
	penetration = currentPen;
	normal = currentNormal;

	for (unsigned i = 1; i < size; i++)
	{
		edge = vertexes[i] - vertexes[(static_cast<unsigned long long>(i) + 1) % size];
		if (!CheckCollideOnOneEdge(edge, other, currentPen, currentNormal))
			return false;

		if (currentPen < penetration)
		{
			penetration = currentPen;
			normal = currentNormal;
		}
	}
	return true;
}

bool Polygon::CheckCollideOnOneEdge(const Vector3& edge, const Polygon& other, float& edgePenetration, Vector3& edgeNormal) const
{
	edgeNormal.x = edge.y;
	edgeNormal.y = -edge.x;
	edgeNormal = edgeNormal.GetUnitVector();

	MinMaxDotAlongNormal ADot(*this, edgeNormal);
	MinMaxDotAlongNormal BDot(other, edgeNormal);

	MinMaxDotAlongNormal* Left;
	MinMaxDotAlongNormal* Right;

	if (ADot.GetMaxDot() >= BDot.GetMaxDot())
	{
		Left = &BDot;
		Right = &ADot;
	}
	else
	{
		Left = &ADot;
		Right = &BDot;
	}

	if (Left->GetMaxDot() < Right->GetMinDot())
		return false;

	edgePenetration = std::abs(Left->GetMaxDot() - Right->GetMinDot());
	return true;
}

void Polygon::SetVertexes(const std::vector<Vector3>& vertexes)
{
	_vertexes = vertexes;
	Vector3 centroid;

	for (const auto& vertex : vertexes)
		centroid += vertex;
	centroid /= static_cast<float>(vertexes.size());

	float pow2Distance = 0;

	for (float currentDisPow2; const auto& vertex : vertexes)
	{
		currentDisPow2 = (vertex - centroid).GetPow2Magnitude();
		if (currentDisPow2 > pow2Distance)
			pow2Distance = currentDisPow2;
	}
	_radius = std::sqrt(pow2Distance);
	_centroid = centroid;
}

const std::vector<Vector3>& Polygon::GetVertexes() const
{
	return _worldVertexes;
}

Polygon::MinMaxDotAlongNormal::MinMaxDotAlongNormal(const Polygon& polygon, const Vector3& normal)
{
	const std::vector<Vector3>& vertexes = polygon.GetVertexes();
	if (vertexes.empty())
		return;

	_maxDot = vertexes[0].Dot(normal);
	_minDot = vertexes[0].Dot(normal);

	unsigned currentMaxIndex = 0;
	unsigned currentMinIndex = 0;

	float currentDot = 0;
	for (int i = 1; i < vertexes.size(); i++)
	{
		currentDot = vertexes[i].Dot(normal);
		if (currentDot < _minDot)
		{
			_minDot = currentDot;
			currentMinIndex = i;
		}
		else if (currentDot > _maxDot)
		{
			_maxDot = currentDot;
			currentMaxIndex = i;
		}
	}

	_maxVertex = vertexes[currentMaxIndex];
	_minVertex = vertexes[currentMinIndex];
}

const Vector3& Polygon::MinMaxDotAlongNormal::GetMaxVertex() const
{
	return _maxVertex;
}

const Vector3& Polygon::MinMaxDotAlongNormal::GetMinVertex() const
{
	return _minVertex;
}

const float& Polygon::MinMaxDotAlongNormal::GetMinDot() const
{
	return _minDot;
}

const float& Polygon::MinMaxDotAlongNormal::GetMaxDot() const
{
	return _maxDot;
}
