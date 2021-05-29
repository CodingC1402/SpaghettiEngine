#pragma once
#include "ScriptBase.h"
#include "Graphics.h"
#include "Camera.h"

class LineRendererBase : public ScriptBase
{
	friend class Collider2DBase;
public:
	LineRendererBase(PScene owner, bool isDisabled = false) : ScriptBase(owner, isDisabled) {};

	virtual void Draw(PCamera camera);
	void SetVertexes(const std::vector<Vector3>& vertextes);
	void SetCircle(float radius, const Vector3& center = Vector3());
	void SetBox(const float width, const float height);
	void SetColor(Color color);
	void SetWidth(const float width);

	void OnEnabled() override;
	void OnDisabled() override;

	virtual void Load(nlohmann::json& inputObject) override;
protected:
	static constexpr unsigned _vertextesForCircle = 20;

	std::vector<Vector3> _vertexes;
	Color _color = ARGB(255, 0, 255, 127);
	float _width = 2;
private:
	REGISTER_START(LineRendererBase);
};