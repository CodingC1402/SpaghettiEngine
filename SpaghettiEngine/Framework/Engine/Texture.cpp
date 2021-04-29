#include "Texture.h"
#include "json.hpp"
#include "Graphics.h"
#include "Sprite.h"
#include <fstream>
#include <sstream>

std::map<unsigned long long, STexture> Texture::textures;

PImage Texture::GetImage() const
{
	return image;
}

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

	std::ifstream jsonFile(_path + ".json");
	if (!jsonFile.is_open()) 
	{
		std::wostringstream os;
		os << L"File ";
		os << _path.c_str();
		os << L" Doesn't exist";
		throw TEXTURE_EXCEPT(os.str());
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
		Graphics::LoadTexture(image, _path, keyColor);

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
		std::wostringstream os;
		os << L"File ";
		os << _path.c_str();
		os << L" doesn't have the right format";
		throw TEXTURE_EXCEPT(os.str());
	}
}

bool Texture::CheckPath(const std::string& path) const
{
	return this->_path == path;
}

bool Texture::GetTexture(STexture* rTexture, const std::string& path)
{
	bool haveToLoadNew = false;
	const unsigned long long findHash = StringConverter::HashStr(path);
	auto texture = textures.find(findHash);
	if (texture == textures.end())
	{
		LoadTexture(path);
		texture = textures.find(findHash);
		haveToLoadNew = true;
	}
	*rTexture = texture->second;
	
	return haveToLoadNew;
}

Texture::~Texture()
{
	image->Release();
	image = nullptr;
}

Texture::Texture(const std::string& path)
{
	this->image = nullptr;
	this->_path = path;
	this->_hash = StringConverter::HashStr(path);
}

void Texture::LoadTexture(const std::string& path)
{
	STexture newTexture = std::make_shared<Texture>(path);
	if (const auto rValue = textures.emplace(newTexture->_hash, newTexture); !rValue.second)
	{
		std::wostringstream os;
		os << L"[Path]" << newTexture->_path.c_str() << std::endl;
		os << L"[Path]" << rValue.first->second->_path.c_str() << std::endl;
		os << L"Have the same hash number please consider using another name";
		throw TEXTURE_EXCEPT(os.str());
	}
	
	newTexture->Load();
}

bool Texture::GetSprite(SSprite* sprite, const int& index) noexcept
{
	if (index >= static_cast<const int>(sprites.size()))
		return false;

	auto iterator = sprites.begin();
	std::advance(iterator, index);
	*sprite = (*iterator);
	return true;
}

void Texture::RemoveTexture(const std::string& path)
{
	const unsigned long long eraseHash = StringConverter::HashStr(path);
	textures.erase(eraseHash);
}

void Texture::ClearUnusedTexture()
{
	std::list<STexture> unusedTextures;
	for(const auto& texture : textures)
		if (texture.second.use_count() <= 1)
			if (texture.second->IsAllSpriteUnused())
				unusedTextures.push_back(texture.second);
	for (const auto& texture : unusedTextures)
		textures.erase(texture->_hash);
}


void Texture::ClearTexture()
{
	textures.clear();
}

Texture::TextureException::TextureException(int line, const char* file, std::wstring description)
	:
	CornDescriptionException(line, file, std::move(description))
{}

const wchar_t* Texture::TextureException::GetType() const noexcept
{
	return L"Texture Exception  ( ◕▿◕ )՞";
}
