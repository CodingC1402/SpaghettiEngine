#include "Sprite.h"

std::vector<SSprite> Sprite::sceneSprite(32);
std::vector<SSprite> Sprite::sceneIndependentSprite(16);

bool Sprite::CheckName(const std::string& name)
{
	return this->name == name;
}

SSprite& Sprite::GetSprite(const std::string& name)
{
	for (int i = 0; i < sceneSprite.size(); i++)
	{
		if (sceneSprite[i]->CheckName(name))
		{
			return sceneSprite[i];
		}
	}
	for (int i = 0; i < sceneIndependentSprite.size(); i++)
	{
		if (sceneIndependentSprite[i]->CheckName(name))
		{
			return sceneIndependentSprite[i];
		}
	}
}

Sprite::Sprite(const STexture& texture, const RECT& sourceRect, const std::string& name)
{
	source = texture;
	this->name = name;
	this->sourceRect = sourceRect;
}

void Sprite::AddSprite(const STexture& texture, const RECT& sourceRect, const std::string& name)
{
	sceneIndependentSprite.push_back(SSprite(new Sprite(texture, sourceRect, name)));
}

void Sprite::RemoveSprite(const std::string& name)
{
	for (int i = 0; i < sceneIndependentSprite.size(); i++)
	{
		if (sceneIndependentSprite[i]->CheckName(name))
		{
			sceneIndependentSprite.erase(sceneIndependentSprite.begin() + i);
			break;
		}
	}
}

void Sprite::AddSceneSprite(const STexture& texture, const RECT& sourceRect, const std::string& name)
{
	sceneSprite.push_back(SSprite(new Sprite(texture, sourceRect, name)));
}

void Sprite::ClearSceneSprite()
{
	sceneSprite.clear();
}
