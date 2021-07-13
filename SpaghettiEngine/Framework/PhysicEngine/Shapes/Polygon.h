#pragma once
#include "Shape.h"
#include <vector>

class Collision;
class Circle;

typedef class Polygon : public Shape
{
public:
	class MinMaxDotAlongNormal
	{
	public:
		MinMaxDotAlongNormal(const Polygon& polygon, const Vector3& normal);

		[[nodiscard]] const Vector3& GetMaxVertex() const;
		[[nodiscard]] const Vector3& GetMinVertex() const;
		
		[[nodiscard]] const float& GetMinDot() const;
		[[nodiscard]] const float& GetMaxDot() const;
	protected:
		Vector3 _minVertex;
		float _minDot = 0;
		Vector3 _maxVertex;
		float _maxDot = 0;
	};
public:
	[[nodiscard]] Type GetType() const override;
	static bool PolygonPolygon(Collision* collision);
	static bool PolygonCircle(Collision* collision);

	/// Used by both polygon-circle and circle-polygon keep it here cause polygon is more complicated than circle so accessing stuff fast
	/// Is better
	static bool PolygonCircleCollision(const Polygon& polygon, const Circle& circle, float& penetration, Vector3& normal);

	void SetVertexes(const std::vector<Vector3>& vertexes);
	[[nodiscard]] const std::vector<Vector3>& GetVertexes() const;
	[[nodiscard]] const std::vector<Vector3>& GetWorldVertexes() const;

	Shape* Clone() const override;
	bool UpdateParameter() override;
protected:
	bool CheckCollideOnOneEdgeWithCircle(const Circle& circle, const Vector3& normal, float& penetration) const ;
	bool CheckCollideOnEachEdge(const Polygon& other, float& penetration, Vector3& normal) const;
	bool CheckCollideOnOneEdge(const Vector3& edge, const Polygon& other, float& edgePenetration, Vector3& edgeNormal) const;
protected:
	std::vector<Vector3> _vertexes;
	std::vector<Vector3> _worldVertexes;
	Vector3 _centroid;
} Polygon2D; //fuck you gdi for stealing polygon name and used for a function : ^)