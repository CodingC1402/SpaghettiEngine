#include "FieldNames.h"

Tag& Fields::SpecialTag::GetPlatformTag()
{
	Load();
	return _platformTag;
}

Tag& Fields::SpecialTag::GetCharacterTag()
{
	Load();
	return _characterTag;
}

Tag& Fields::SpecialTag::GetPlayerTag()
{
	Load();
	return _playerTag;
}

Tag& Fields::SpecialTag::GetHeadTag()
{
	Load();
	return _headTag;
}

Tag& Fields::SpecialTag::GetFeetTag()
{
	Load();
	return _feetTag;
}

Tag& Fields::SpecialTag::GetEnemyTag()
{
	Load();
	return _enemyTag;
}

Tag& Fields::SpecialTag::GetStarOrCharacterTag()
{
	Load();
	return _starOrCharacter;
}

void Fields::SpecialTag::Load()
{
	if (_isLoaded)
		return;

	_platformTag	= Tag(Platform::_platform);
	_characterTag	= Tag(Character::_character);
	_playerTag		= Tag(Player::_player);
	_headTag		= Tag(Player::_head);
	_feetTag		= Tag(Player::_feet);
	_enemyTag		= Tag(Enemy::_enemy);
	_starOrCharacter= Tag(std::string(Character::_character) + Tag::GetDelimeter() + Star::_star);

	_isLoaded = true;
}

