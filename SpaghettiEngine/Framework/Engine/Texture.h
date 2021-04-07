#pragma once
#include "json.hpp"
#include "Sprite.h"
#include "CornException.h"
#include <string>
#include <d3d9.h>
#include <memory>
#include <list>

typedef class Texture* PTexture;
typedef std::shared_ptr<Texture> STexture;
typedef std::unique_ptr<Texture> UTexture;

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

typedef LPDIRECT3DTEXTURE9 PDx9Texture;

class Texture
{
public:
	class TextureException : public CornDiscriptionException
	{
	public:
		TextureException(int line, const char* file, std::wstring discription);
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static bool GetTexture(STexture& texture, const std::string& path) noexcept;
	static void AddTexture(const std::string& path);
	bool GetSprite(SSprite& sprite, const int& index) noexcept;
	~Texture();
protected:
	Texture(const std::string& path);
	static void RemoveTexture(const std::string& path);
	static void ClearUnusedTexture();
	static void ClearTexture();

	void Load();
	bool CheckPath(const std::string& path);
protected:
	PDx9Texture image;
	std::string path;

	std::list<SSprite>sprites;
	static std::list<STexture> textures;
};

#define TEXTURE_EXCEPT(discription) Texture::TextureException(__LINE__,__FILE__,discription)