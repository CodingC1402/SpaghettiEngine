#pragma once
#include "json.hpp"
#include <string>
#include <d3d9.h>
#include <memory>
#include <list>

typedef class Texture* PTexture;
typedef std::shared_ptr<Texture> STexture;
typedef std::unique_ptr<Texture> UTexture;

typedef LPDIRECT3DTEXTURE9 PDx9Texture;

class Texture
{
public:
	static bool GetTexture(STexture& texture, const std::string& path) noexcept;
	static void AddTexture(const std::string& path);
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

	std::list<SSprite>

	static std::list<STexture> texture;
};
