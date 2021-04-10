#include "Texture.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "Graphics.h"

std::list<STexture> Texture::textures;

PDx9Texture Texture::GetImage()
{
	return image;
}

void Texture::Load()
{
	using namespace nlohmann;

	std::ifstream jsonFile(path);
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

		D3DCOLOR keyColor = file["KeyColor"].get<D3DCOLOR>();
		Graphics::LoadTexture(image, path, keyColor);

		int size = file["Size"].get<int>();
		int x;
		int y;
		int w;
		int h;
		std::string index;
		for (int i = 0; i < size; i++)
		{
			index = static_cast<char>('0' + i);
			x = file[index]['X'].get<int>();
			y = file[index]['Y'].get<int>();
			w = file[index]['W'].get<int>();
			h = file[index]['H'].get<int>();
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
	
	LoadTexture(path);
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

Texture::TextureException::TextureException(int line, const char* file, std::wstring discription)
	:
	CornDiscriptionException(line, file, discription)
{}

const wchar_t* Texture::TextureException::GetType() const noexcept
{
	return L"Texture Exception  ( ◕▿◕ )՞";
}
