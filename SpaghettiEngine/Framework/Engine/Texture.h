#pragma once
#include "json.hpp"
#include "CornException.h"
#include "CornDirectX.h"
#include <string>
#include <memory>
#include <map>

typedef class Texture* PTexture;
typedef std::shared_ptr<Texture> STexture;
typedef std::unique_ptr<Texture> UTexture;

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

typedef LPDIRECT3DTEXTURE9 PDx9Texture;

class Texture
{
	friend class SceneManager;
public:
	class TextureException : public CornDescriptionException
	{
	public:
		TextureException(int line, const char* file, std::wstring description);
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static bool GetTexture(STexture* texture, const std::string& path);
	static void LoadTexture(const std::string& path);

	bool GetSprite(SSprite* sprite, const int& index) noexcept;
	[[nodiscard]]PDx9Texture GetImage() const;
	Texture(const std::string& path);
	~Texture();
protected:
	static void RemoveTexture(const std::string& path);
	static void ClearUnusedTexture();
	static void ClearTexture();

	bool IsAllSpriteUnused();
	void Load();
	bool CheckPath(const std::string& path) const;
protected:
	PDx9Texture image;
	std::string _path;
	unsigned long long _hash;
	
	std::list<SSprite>sprites;
	static std::map<unsigned long long, STexture> textures;
};

#define TEXTURE_EXCEPT(description) Texture::TextureException(__LINE__,__FILE__,description)