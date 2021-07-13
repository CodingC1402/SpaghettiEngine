#pragma once
#include "Collider2DBase.h"
#include "Polygon.h"

typedef std::shared_ptr<Polygon2D> SPolygon;

class Polygon2DCollider : public Collider2DBase
{
public:
	void OnFixedUpdate();
	void Load(nlohmann::json& input) override;
private:
	REGISTER_START(Polygon2DCollider);
};