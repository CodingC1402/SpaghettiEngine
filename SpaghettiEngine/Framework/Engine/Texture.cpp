#include "Texture.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "Graphics.h"
#include "GraphicsMath.h"

std::list<STexture> Texture::textures;

PDx9Texture Texture::GetImage()
{
	return image;
}

#define KEYCOLOR "KeyColor"
#define RED "Red"
#define GREEN "Green"
#define BLUE "Blue"
#define Sprites "Sprites"
#define SpritePosX 0
#define SpritePosY 1
#define SpriteWidth 2
#define SpriteHeight 3

bool Texture::IsAllSpriteUnused()
{
	for (const auto& sprite : sprites)
	{
		if (sprite.use_count() > 1)
			return false;
	}
	return true;
}

void Texture::Load()
{
	using namespace nlohmann;

	std::ifstream jsonFile(path + ".json");
	if (!jsonFile.is_open()) 
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" Doesn't exist";
		throw TEXTURE_EXCEPT(os.str());
	}

	try
	{
		json file;
		jsonFile >> file;

		UINT red = file[KEYCOLOR][RED].get<int>();
		UINT green = file[KEYCOLOR][GREEN].get<int>();
		UINT blue = file[KEYCOLOR][BLUE].get<int>();
		auto keyColor = ARGB(red, green, blue, 255);
		Graphics::LoadTexture(image, path, keyColor);

		for (int x, y, w, h; const auto& sprite : file["Sprites"])
		{
			x = sprite[SpritePosX].get<int>();
			y = sprite[SpritePosY].get<int>();
			w = sprite[SpriteWidth].get<int>();
			h = sprite[SpriteHeight].get<int>();
			sprites.push_back(SSprite(new Sprite(this, x, y, w, h)));
		}
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << path.c_str();
		os << L" doesn't have the right format";
		throw TEXTURE_EXCEPT(os.str());
	}
}

bool Texture::CheckPath(const std::string& path)
{
	return this->path == path;
}

bool Texture::GetTexture(STexture* rTexture, const std::string& path)
{
	for (const auto& texture : textures)
	{
		if (texture->CheckPath(path))
		{
			*rTexture = texture;
			return true;
		}
	}
	
	LoadTexture(path);
	GetTexture(rTexture, path);
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

void Texture::LoadTexture(const std::string& path)
{
	textures.push_back(STexture(new Texture(path)));
	textures.back()->Load();
}

bool Texture::GetSprite(SSprite* sprite, const int& index) noexcept
{
	if (index >= sprites.size())
		return false;

	auto iterator = sprites.begin();
	std::advance(iterator, index);
	*sprite = (*iterator);
	return true;
}

void Texture::RemoveTexture(const std::string& path)
{
	auto iterator = textures.begin();
	size_t size = textures.size();
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
	size_t size = textures.size();
	auto iterator = textures.begin();
	while (size > 0)
	{
		if (iterator->use_count() <= 1)
		{
			if ((*iterator)->IsAllSpriteUnused())
			{
				auto eraseIterator = iterator;
				std::advance(iterator, 1);
				textures.erase(eraseIterator);
			}
		}
		size--;
	}
}


void Texture::ClearTexture()
{
	textures.clear();
}

Texture::TextureException::TextureException(int line, const char* file, std::wstring discription)
	:
	CornDiscriptionException(line, file, discription)
{}

const wchar_t* Texture::TextureException::GetType() const noexcept
{
	return L"Texture Exception  ( ◕▿◕ )՞";
}
