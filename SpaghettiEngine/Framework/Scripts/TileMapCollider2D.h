#pragma once
#include "Collider2DScriptBase.h"

typedef class TileMapCollider2D* PTileMapCollider2D;

class TileMapCollider2D : public ScriptBase
{
public:
	TileMapCollider2D(PScene owner);

	virtual void Load(nlohmann::json& input) override;
//protected:
//	std::list<PCollider2DScriptBase> colliders;
private:
	REGISTER_START(TileMapCollider2D);
};