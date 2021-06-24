#include "Polygon.h"
#include "Collision.h"
#include "Circle.h"
#include "SMath.h"
#include <cmath>
#include <limits>

Shape::Type Polygon::GetType() const
{
	return Type::Polygon;
}

bool Polygon::PolygonPolygon(Collision* collision)
{
	bool result = false;
	const auto shapeA = dynamic_cast<Polygon*>(collision->GetShapeA());
	const auto shapeB = dynamic_cast<Polygon*>(collision->GetShapeB());

	Vector3 normalA;
	Vector3 normalB;
	float penetrationA = 0;
	float penetrationB = 0;

	Vector3 normal;
	float penetration = 0;

	result = shapeA->CheckCollideOnEachEdge(*shapeB, penetrationA, normalA);
	if (!result)
		return false;

	result = shapeB->CheckCollideOnEachEdge(*shapeA, penetrationB, normalB);
	if (!result)
		return false;

	if (penetrationA < penetrationB)
	{
		normal = normalA;
		penetration = penetrationA;
	}
	else
	{
		normal = normalB;
		penetration = penetrationB;
	}

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

	Vector3 normal;
	float penetration;

	if (!PolygonCircleCollision(*polygon, *circle, penetration, normal))
		return false;

	if (normal.Dot(collision->GetShapeB()->GetCenter() - collision->GetShapeA()->GetCenter()) < 0)
		normal = -normal;

	collision->SetPenetration(penetration);
	collision->SetNormal(normal);

	return true;
}

bool Polygon::PolygonCircleCollision(const Polygon& polygon, const Circle& circle, float& penetrationOut, Vector3& normalOut)
{
	float penetration;

	float distance = std::numeric_limits<float>::max();
	Vector3 closestVertex;

	float leastPenetration = std::numeric_limits<float>::max();
	Vector3 leastPenNormal;

	auto Vertexes = polygon.GetVertexes();
	// Check on normal of edges
	unsigned numberOfVertex = Vertexes.size();
	for (int i = 0; i < numberOfVertex; i++)
	{
		float currentDis = (Vertexes[i] - circle.GetCenter()).GetPow2Magnitude();
		if (currentDis < distance)
		{
			distance = currentDis;
			closestVertex = Vertexes[i];
		}

		Vector3 edge = Vertexes[i] - Vertexes[(i + 1) % numberOfVertex];
		Vector3 normalUnit = Vector3(edge.y, -edge.x, 0).GetUnitVector();

		if (!polygon.CheckCollideOnOneEdgeWithCircle(circle, normalUnit, penetration))
			return false;

		if (penetration < leastPenetration)
		{
			leastPenetration = penetration;
			leastPenNormal = normalUnit;
		}
	}

	// Check on normal of connect line between center and closest corner
	Vector3 connectEdge = (closestVertex - circle.GetCenter()).GetUnitVector();
	if (!polygon.CheckCollideOnOneEdgeWithCircle(circle, connectEdge, penetration))
		return false;

	if (penetration < leastPenetration)
	{
		leastPenetration = penetration;
		leastPenNormal = connectEdge;
	}

	penetrationOut = leastPenetration;
	normalOut = leastPenNormal;
	return true;
}

Shape* Polygon::Clone() const
{
	Polygon* clonePoly = new Polygon();

	clonePoly->_center = _center;
	clonePoly->_radius = _radius;
	clonePoly->_centroid = _centroid;
	clonePoly->_offSetMatrix = _offSetMatrix;
	clonePoly->_vertexes = _vertexes;

	return clonePoly;
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


bool Polygon::CheckCollideOnOneEdgeWithCircle(const Circle& circle, const Vector3& normal, float& penetration) const
{
	MinMaxDotAlongNormal PolyDot(*this, normal);

	float centerDot = circle.GetCenter().Dot(normal);
	float centerMaxDot = centerDot + circle.GetRadius();
	float centerMinDot = centerDot - circle.GetRadius();

	float biggerMax, biggerMin;
	float smallerMax, smallerMin;

	if (centerMaxDot > PolyDot.GetMaxDot())
	{
		biggerMax = centerMaxDot;
		biggerMin = centerMinDot;

		smallerMax = PolyDot.GetMaxDot();
		smallerMin = PolyDot.GetMinDot();
	}
	else
	{
		biggerMax = PolyDot.GetMaxDot();
		biggerMin = PolyDot.GetMinDot();

		smallerMax = centerMaxDot;
		smallerMin = centerMinDot;
	}

	if (smallerMax < biggerMin)
		return false;

	penetration = std::abs(smallerMax - biggerMin);
	return true;
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
