#include "Enemy.h"
#include "FieldNames.h"

REGISTER_FINISH(Enemy, ScriptBase) {}

void Enemy::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		if (e.GetNormal().y < 0.75)
			e.SetIsHandled(true);
	}
	else if (e.GetGameObject()->GetTag().Collide(Fields::SpecialTag::GetEnemyTag()))
	{
		e.SetIsHandled(true);
	}
		
}

void Enemy::OnUpdate()
{
	
}
