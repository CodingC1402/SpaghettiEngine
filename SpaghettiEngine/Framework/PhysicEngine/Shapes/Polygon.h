#pragma once
#include "Shape.h"
#include <vector>

class Collision;

typedef class Polygon : public Shape
{
public:
	class MinMaxDotAlongNormal
	{
	public:
		MinMaxDotAlongNormal(const Polygon& polygon, const Vector3& normal);

		const Vector3& GetMaxVertex() const;
		const Vector3& GetMinVertex() const;

		const float& GetMinDot() const;
		const float& GetMaxDot() const;
	protected:
		Vector3 _minVertex;
		float _minDot = 0;
		Vector3 _maxVertex;
		float _maxDot = 0;
	};
public:
	Type GetType() const override;
	static bool PolygonPolygon(Collision* collision);
	static bool PolygonCircle(Collision* collision);

	void SetVertexes(const std::vector<Vector3>& vertexes);
	[[nodiscard]] const std::vector<Vector3>& GetVertexes() const;

	void UpdateParameter() override;
protected:
	bool CheckCollideOnEachEdge(const Polygon& other, float& penatration, Vector3& normal);
	bool CheckCollideOnOneEdge(const Vector3& edge, const Polygon& other, float& edgePenatration, Vector3& edgeNormal);
protected:
	std::vector<Vector3> _vertexes;
	std::vector<Vector3> _worldVertexes;
	Vector3 _centroid;
} Polygon2D; //fuck you gdi for stealling polygon name and used for a function : ^)