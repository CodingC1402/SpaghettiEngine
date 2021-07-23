#include "Enemy.h"
#include "FieldNames.h"

REGISTER_FINISH(Enemy, ScriptBase) {}

void Enemy::OnDisabled()
{
	if (_isInTube)
		GetGameObject()->CallDestroy();
}

void Enemy::OnCollide(CollideEvent& e)
{
	if (e.GetGameObject()->GetTag().Collide(_collideIgnore))
	{
		e.SetIsHandled(true);
	}
}

void Enemy::Load(nlohmann::json& input)
{
	_collideIgnore = input[Fields::Enemy::_ignore].empty() ? "" : Tag(input[Fields::Enemy::_ignore]);
}

void Enemy::SetIsInTube()
{
	_isInTube = true;
}

ScriptBase* Enemy::Clone() const
{
	auto clone = dynamic_cast<Enemy*>(ScriptBase::Clone());

	clone->_collideIgnore = _collideIgnore;
	clone->_isInTube = _isInTube;

	return clone;
}

bool Enemy::IsInTube()
{
	return _isInTube;
}
