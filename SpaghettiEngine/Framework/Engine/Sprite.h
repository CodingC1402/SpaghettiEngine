#pragma once
#include "Texture.h"
#include "Plane2D.h"

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

class Texture;
typedef std::shared_ptr<Texture> STexture;

class Sprite
{
public:
	static bool GetSprite(SSprite& rSprite, const std::string& path);
	static void AddSprite(const std::string& path);
protected:
	Sprite(const std::string&  path);
	static void RemoveSprite(const std::string& path);
	static void ClearUnusedSprite();
	static void ClearSprite();

	bool CheckPath(const std::string& path);
protected:
	Plane2D::Size size;
	Plane2D::Point position;
	STexture source;
	std::string path;

	static std::list<SSprite> sprite;
};

