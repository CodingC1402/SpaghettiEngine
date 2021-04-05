#include "Sprite.h"

std::vector<SSprite> Sprite::loadedSprite(32);

bool Sprite::CheckName(const std::string& name)
{
	return this->name == name;
}

SSprite Sprite::GetSprite(const std::string& name)
{
	return SSprite();
}

Sprite::Sprite(const STexture& texture, const RECT& sourceRect, const std::string& name)
{
	source = texture;
	this->name = name;
	this->sourceRect = sourceRect;
}

void Sprite::AddSprite(const STexture& texture, const RECT& sourceRect, const std::string& name)
{
	loadedSprite.push_back(SSprite(new Sprite(texture, sourceRect, name)));
}

void Sprite::RemoveSprite(const std::string& name)
{
	for (int i = 0; i < loadedSprite.size(); i++)
	{
		if (loadedSprite[i]->CheckName(name))
		{
			loadedSprite.erase(loadedSprite.begin() + i);
		}
	}
}

void Sprite::ClearSprite()
{
	loadedSprite.clear();
}
