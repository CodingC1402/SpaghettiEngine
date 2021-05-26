#include "Texture.h"
#include "json.hpp"
#include "Graphics.h"
#include "Sprite.h"
#include "SpaghettiEnginePath.h"
#include <fstream>
#include <sstream>

CONTAINER_REGISTER(TextureContainer, Texture);

PImage Texture::GetImage() const
{
	return image;
}

void Texture::Load(const std::string& path)
{	
	using namespace nlohmann;

	std::ifstream jsonFile(path + ".json");
	if (!jsonFile.is_open()) 
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Texture);
	}

	try
	{
		constexpr const char* KeyColor = "KeyColor";
		constexpr const char* Sprites = "Sprites";
		constexpr const char* Red = "Red";
		constexpr const char* Green = "Green";
		constexpr const char* Blue = "Blue";
		
		json file;
		jsonFile >> file;

		UINT red = file[KeyColor][Red].get<int>();
		UINT green = file[KeyColor][Green].get<int>();
		UINT blue = file[KeyColor][Blue].get<int>();
		auto keyColor = ARGB(red, green, blue, 255);
		Graphics::LoadTexture(image, path, keyColor);

		sprites.reserve(file[Sprites].size());
		for (int x, y, w, h; const auto& sprite : file[Sprites])
		{
			constexpr int SpritePosX = 0;
			constexpr int SpritePosY = 1;
			constexpr int SpriteWidth = 2;
			constexpr int SpriteHeight = 3;
			
			x = sprite[SpritePosX].get<int>();
			y = sprite[SpritePosY].get<int>();
			w = sprite[SpriteWidth].get<int>();
			h = sprite[SpriteHeight].get<int>();
			sprites.emplace_back(SSprite(new Sprite(this, x, y, w, h)));
		}
	}
	catch (...)
	{
		std::ostringstream os;
		os << "File ";
		os << path.c_str();
		os << " doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Texture);
	}
}

Texture::Texture() : Resource(), image(nullptr)
{}

Texture::~Texture()
{
	image->Release();
	image = nullptr;
}

SSprite Texture::GetSprite(const unsigned int& index) noexcept
{
	if (index >= static_cast<const unsigned int>(sprites.size()))
		return SSprite();
	
	return sprites[index];
}

std::vector<SSprite> Texture::GetAllSprites() noexcept
{
	return sprites;
}

unsigned Texture::GetSpritesNumber() noexcept
{
	return sprites.size();
}

TextureContainer::TextureContainer()
{
	_name = RESOURCE_NAME(Texture);
	LoadEntries(SystemPath::TextureEntriesPath);
}

bool ::Texture::IsResourceUnused() const
{
	for (const auto& sprite : sprites)
		if (sprite.use_count() > 1)
			return false;
	return true;
}
