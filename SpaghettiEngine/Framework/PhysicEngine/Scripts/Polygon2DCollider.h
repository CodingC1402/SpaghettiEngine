#pragma once
#include "Collider2DBase.h"
#include "Polygon.h"

typedef std::shared_ptr<Polygon2D> SPolygon;

class Polygon2DCollider : public Collider2DBase
{
public:
	Polygon2DCollider(PScene owner, bool isDisabled = false);

	void OnFixedUpdate();

	void Load(nlohmann::json& input) override;
protected:
	static constexpr auto _vertexesField = "Vertexes";
private:
	REGISTER_START(Polygon2DCollider);
};