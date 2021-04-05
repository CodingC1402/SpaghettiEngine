#pragma once
#include "json.hpp"
#include <string>
#include <d3d9.h>
#include <memory>
#include <vector>

typedef class Texture* PTexture;
typedef std::shared_ptr<Texture> STexture;
typedef std::unique_ptr<Texture> UTexture;

typedef LPDIRECT3DSURFACE9 PSurface;

class Texture
{
	friend class Graphics;
public:
	bool CheckName(const std::string& name);
	~Texture();
protected:
	Texture(PSurface image, std::string name);
	static void AddTexture(PSurface surface, std::string name);
	static void RemoveTexture(std::string name);
	static void ClearTexture();
protected:
	PSurface image;
	std::string name;

	static std::vector<STexture> loadedTexture;
};
