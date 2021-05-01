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
		static constexpr const char* KeyColor = "KeyColor";
		static constexpr const char* Sprites = "Sprites";
		static constexpr const char* Red = "Red";
		static constexpr const char* Green = "Green";
		static constexpr const char* Blue = "Blue";
		
		json file;
		jsonFile >> file;

		UINT red = file[KeyColor][Red].get<int>();
		UINT green = file[KeyColor][Green].get<int>();
		UINT blue = file[KeyColor][Blue].get<int>();
		auto keyColor = ARGB(red, green, blue, 255);
		Graphics::LoadTexture(image, path, keyColor);

		for (int x, y, w, h; const auto& sprite : file[Sprites])
		{
			static constexpr int SpritePosX = 0;
			static constexpr int SpritePosY = 1;
			static constexpr int SpriteWidth = 2;
			static constexpr int SpriteHeight = 3;
			
			x = sprite[SpritePosX].get<int>();
			y = sprite[SpritePosY].get<int>();
			w = sprite[SpriteWidth].get<int>();
			h = sprite[SpriteHeight].get<int>();
			sprites.push_back(SSprite(new Sprite(this, x, y, w, h)));
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

SSprite Texture::GetSprite(const int& index) noexcept
{
	if (index >= static_cast<const int>(sprites.size()))
		return SSprite();

	auto iterator = sprites.begin();
	std::advance(iterator, index);
	return (*iterator);
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
