#pragma once
#include "Vector3.h"
#include <vector>

//
// Store data used for QuadTree in AABB Rect form
//

class BroadPhaseShape
{
public:
	void SetRadius(float radius) noexcept;
	void SetVertexes(const std::vector<Vector3>& vertexes) noexcept;

	void UpdateMatrix(const Matrix4& worldMatrix, const Matrix4& offSet) noexcept;

	[[nodiscard]] const Vector3& GetCenter() const noexcept;
	[[nodiscard]] const Vector3& GetTopLeft() const noexcept;
	[[nodiscard]] const Vector3& GetBottomRight() const noexcept;

	[[nodiscard]] const Vector3& GetWorldCenter() const noexcept;
	[[nodiscard]] const Vector3& GetWorldTopLeft() const noexcept;
	[[nodiscard]] const Vector3& GetWorldBottomRight() const noexcept;

	[[nodiscard]] float GetWidth() const noexcept;
	[[nodiscard]] float GetHeight() const noexcept;
protected:
	void SetCenter() noexcept;
protected:
	Vector3 _topLeft;
	Vector3 _bottomRight;
	Vector3 _center;
	float _width;
	float _height;

	Vector3 _worldTopLeft;
	Vector3 _worldBottomRight;
	Vector3 _worldCenter;
};