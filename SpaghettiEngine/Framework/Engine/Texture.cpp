#include "Texture.h"
#include "json.hpp"
#include <fstream>

std::list<STexture> Texture::textures;

void Texture::Load()
{

}

bool Texture::CheckPath(const std::string& path)
{
	return this->path == path;
}

bool Texture::GetTexture(STexture& rTexture, const std::string& path) noexcept
{
	int size = textures.size();
	auto iterator= textures.begin();
	while (size > 0)
	{
		if ((*iterator)->CheckPath(path))
		{
			rTexture = (*iterator);
			return true;
		}
		size--;
	}
	return false;
}

Texture::~Texture()
{
	image->Release();
	image = nullptr;
}

Texture::Texture(const std::string& path)
{
	this->path = path;
}

void Texture::AddTexture(const std::string& path)
{
	textures.push_back(STexture(new Texture(path)));
}

bool Texture::GetSprite(SSprite& sprite, const int& index) noexcept
{
	if (index >= sprites.size())
		return false;

	auto iterator = sprites.begin();
	std::advance(iterator, index);
	sprite = (*iterator);
	return true;
}

void Texture::RemoveTexture(const std::string& path)
{
	auto iterator = textures.begin();
	int size = textures.size();
	while (size > 0)
	{
		if ((*iterator)->CheckPath(path))
		{
			textures.erase(iterator);
			break;
		}
		size--;
	}
}

void Texture::ClearUnusedTexture()
{
	int size = textures.size();
	auto iterator = textures.begin();
	while (size > 0)
	{
		if (iterator->use_count() == 1)
		{
			auto eraseIterator = iterator;
			std::advance(iterator, 1);
			textures.erase(eraseIterator);
		}
		size--;
	}
}

void Texture::ClearTexture()
{
	textures.clear();
}

