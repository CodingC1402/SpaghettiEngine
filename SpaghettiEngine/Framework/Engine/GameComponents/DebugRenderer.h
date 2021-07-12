#pragma once
#include "Vector3.h"
#include "Color.h"
#include "Camera.h"

#include <vector>
#include <list>

class DirectX9Graphic;
typedef std::shared_ptr<DirectX9Graphic> SDirectX9Graphic;

class DebugRenderer
{
	friend class Physic;
	friend class Graphics; // Manage to let this class know when to draw.
public:
	static void DrawPolygon(const std::vector<Vector3>& vertexes, const Matrix4& matrix);
	static void DrawCircle(const float& radius, const Matrix4& matrix);
	static void SetColor(const Color& color) noexcept;
	static void SetWidth(const float& width) noexcept;

	[[nodiscard]] static Color GetColor() noexcept;
	[[nodiscard]] static float GetWidth() noexcept;
protected:
	static void Render(SDirectX9Graphic dxTurd, PCamera cameraScript);
	static void Clear() noexcept;
protected:
	static inline std::list<std::pair<std::vector<Vector3>, Matrix4>> _shapes;
	static inline Color _renderColor = Color(0, 255, 255, 255);
	static inline float _width = 1;

	static constexpr unsigned _maxVertexNumberForCircle = 20;
	static constexpr unsigned _minVertexNumberForCircle = 10;
	static constexpr unsigned _radiusToReachMaxVertexNum = 100;
};