#include "Sprite.h"

std::list<SSprite> Sprite::sprite;

bool Sprite::GetSprite(SSprite& rSprite, const std::string& path)
{
	return false;
}

Sprite::Sprite(const std::string& path)
{
	this->path = path;
}

void Sprite::RemoveSprite(const std::string& path)
{
	int size = sprite.size();
	auto iterator = sprite.begin();
	while (size > 0)
	{
		if ((*iterator)->CheckPath(path))
		{
			sprite.erase(iterator);
			break;
		}
		size--;
	}
}

void Sprite::AddSprite(const std::string& path)
{
	SSprite newSprite = SSprite(new Sprite(path));
	sprite.push_back(newSprite);
}

void Sprite::ClearUnusedSprite()
{
	int size = sprite.size();
	auto iterator = sprite.begin();
	while (size > 0)
	{
		if (iterator->use_count() == 1)
		{
			auto eraseIterator = iterator;
			std::advance(iterator, 1);
			sprite.erase(eraseIterator);
		}
		size--;
	}
}

void Sprite::ClearSprite()
{
	sprite.clear();
}

bool Sprite::CheckPath(const std::string& path)
{
	return this->path == path;
}
