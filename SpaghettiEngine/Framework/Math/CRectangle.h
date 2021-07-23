#pragma once 
#include "Vector3.h"
#include "json.hpp"

// This is only use for simplicity don't use this for algorithms
class CRectangle
{
public:
	[[nodiscard]] const Vector3& GetBottomLeft() const noexcept;
	[[nodiscard]] const Vector3& GetTopRight() const noexcept;
	[[nodiscard]] const Vector3& GetCenter() const noexcept;
	CRectangle& operator=(const CRectangle& rect) noexcept;

	[[nodiscard]] float GetMaxX() const noexcept;
	[[nodiscard]] float GetMaxY() const noexcept;
	[[nodiscard]] float GetMinX() const noexcept;
	[[nodiscard]] float GetMinY() const noexcept;

	[[nodiscard]] float GetWidth() const noexcept;
	[[nodiscard]] float GetHeight() const noexcept;

	void SetCenter(const Vector3& center);
	void SetWidth(const float& width);
	void SetHeight(const float& height);

	CRectangle() = default;
	CRectangle(const Vector3& center, const float& width, const float& height);
	CRectangle(nlohmann::json& input);
private:
	Vector3 _bottomLeft;
	Vector3 _topRight;
	Vector3 _center;
	float _width = 0;
	float _height = 0;

	static constexpr auto _centerField = "Center";
	static constexpr auto _widthField = "Width";
	static constexpr auto _heightField = "Height";
};