#include "Enemy.h"
#include "FieldNames.h"

REGISTER_FINISH(Enemy, ScriptBase) {}

void Enemy::OnStart()
{
	_collideIgnore = Fields::SpecialTag::GetEnemyTag() | Fields::SpecialTag::GetPlayerAttack() | Fields::SpecialTag::GetPlayerTag();
}

void Enemy::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(_collideIgnore))
	{
		e.SetIsHandled(true);
	}
		
}

void Enemy::SetIsInTube()
{
	_isInTube = true;
}

bool Enemy::IsInTube()
{
	return _isInTube;
}
