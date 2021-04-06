#include "Texture.h"

std::list<STexture> Texture::texture;

void Texture::Load()
{

}

bool Texture::CheckPath(const std::string& path)
{
	return this->path == path;
}

bool Texture::GetTexture(STexture& rTexture, const std::string& path) noexcept
{
	int size = texture.size();
	auto iterator= texture.begin();
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
	texture.push_back(STexture(new Texture(path)));
}

void Texture::RemoveTexture(const std::string& path)
{
	auto iterator = texture.begin();
	int size = texture.size();
	while (size > 0)
	{
		if ((*iterator)->CheckPath(path))
		{
			texture.erase(iterator);
			break;
		}
		size--;
	}
}

void Texture::ClearUnusedTexture()
{
	int size = texture.size();
	auto iterator = texture.begin();
	while (size > 0)
	{
		if (iterator->use_count() == 1)
		{
			auto eraseIterator = iterator;
			std::advance(iterator, 1);
			texture.erase(eraseIterator);
		}
		size--;
	}
}

void Texture::ClearTexture()
{
	texture.clear();
}

