#pragma once
#include "json.hpp"
#include <string>
#include <d3d9.h>
#include <memory>
#include <vector>

typedef class Texture* PTexture;
typedef std::shared_ptr<Texture> STexture;
typedef std::unique_ptr<Texture> UTexture;

typedef LPDIRECT3DTEXTURE9 PDx9Texture;

class Texture
{
	friend class Scene;
	friend class Graphics;
public:
	static STexture& GetTexture(const std::string& name) noexcept;
	~Texture();
protected:
	Texture(const PDx9Texture& image, const std::string& name);
	static void AddSceneTexture(const PDx9Texture& image, const std::string& name);
	static void ClearSceneTexture();

	static void AddTexture(const PDx9Texture& image, const std::string& name);
	static void RemoveTexture(const std::string& name);

	bool CheckName(const std::string& name);
protected:
	PDx9Texture image;
	std::string name;

	static std::vector<STexture> sceneTexture;
	static std::vector<STexture> sceneIndependentTexture;
};
