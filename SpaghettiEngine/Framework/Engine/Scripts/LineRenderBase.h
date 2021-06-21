#pragma once
#include "ScriptBase.h"
#include "Color.h"

typedef class Camera* PCamera;

class LineRendererBase : public ScriptBase
{
	friend class Collider2DBase;
public:
	LineRendererBase(PScene owner, bool isDisabled = false);

	virtual void Draw(PCamera camera);
	void SetVertexes(const std::vector<Vector3>& vertextes);
	void SetCircle(float radius, const Vector3& center = Vector3());
	void SetBox(const float width, const float height);
	void SetColor(Color color);
	void SetWidth(const float width);
	void SetOffSetX(const float x);
	void SetOffSetY(const float y);

	void OnEnabled() override;
	void OnDisabled() override;

	virtual void Load(nlohmann::json& inputObject) override;
	SScriptBase Clone() const override;
protected:
	static constexpr unsigned _vertextesForCircle = 50;
	static constexpr unsigned _minVertexesForCircle = 10;
	static constexpr float _radiusToMaxVertexesNumber = 200;

	Matrix4 _offSetMatrix;
	std::vector<Vector3> _vertexes;
	Color _color = GREEN;
	float _width = 2;
private:
	REGISTER_START(LineRendererBase);
};