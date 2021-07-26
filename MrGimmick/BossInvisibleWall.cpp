#include "BossInvisibleWall.h"

REGISTER_FINISH(BossInvisibleWall, ScriptBase) {}

void BossInvisibleWall::OnCollide(CollideEvent& e)
{
	if (!e.GetGameObject()->GetTag().Contain("Boss"))
	{
		e.SetIsHandled(true);
	}
}
