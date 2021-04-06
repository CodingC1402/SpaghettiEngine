#include "Texture.h"

std::vector<STexture> Texture::sceneTexture(16);
std::vector<STexture> Texture::sceneIndependentTexture(16);

bool Texture::CheckName(const std::string& name)
{
	return this->name == name;
}

STexture& Texture::FindTexture(const std::string& name) noexcept
{
	for (int i = 0; i < sceneTexture.size(); i++)
	{
		if (sceneTexture[i]->CheckName(name))
			return sceneTexture[i];
	}
}

Texture::~Texture()
{
	image->Release();
	image = nullptr;

	sceneTexture.clear();
	sceneIndependentTexture.clear();
}

Texture::Texture(const PDx9Texture& image, const std::string& name)
{
	this->image = image;
	this->name = name;
}

void Texture::AddSceneTexture(const PDx9Texture& image, const std::string& name)
{
	sceneTexture.push_back(STexture(new Texture(image, name)));
}

void Texture::ClearSceneTexture()
{
	sceneTexture.clear();
}

void Texture::AddTexture(const PDx9Texture& image, const std::string& name)
{
	sceneIndependentTexture.push_back(STexture(new Texture(image, name)));
}

void Texture::RemoveTexture(const std::string& name)
{
	for (int i = 0; i < sceneIndependentTexture.size(); i++)
	{
		if (sceneIndependentTexture[i]->CheckName(name))
		{
			sceneIndependentTexture.erase(sceneIndependentTexture.begin() + i);
			break;
		}
	}
}

