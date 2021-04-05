#include "Texture.h"

std::vector<STexture> Texture::loadedTexture(16);

bool Texture::CheckName(const std::string& name)
{
	return this->name == name;
}

Texture::~Texture()
{
	image->Release();
	image = nullptr;
}

Texture::Texture(PSurface image, std::string name)
{
	this->image = image;
	this->name = name;
}

void Texture::AddTexture(PSurface image, std::string name)
{
	loadedTexture.push_back(STexture(new Texture(image, name)));
}

void Texture::RemoveTexture(std::string name)
{
	for (int i = 0; i < loadedTexture.size(); i++)
	{
		if (loadedTexture[i]->CheckName(name))
		{
			loadedTexture.erase(loadedTexture.begin() + i);
		}
	}
}

void Texture::ClearTexture()
{
	loadedTexture.clear();
}
