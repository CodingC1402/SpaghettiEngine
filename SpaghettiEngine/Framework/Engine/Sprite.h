#pragma once
#include "Texture.h"
#include "Plane2D.h"

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

class Sprite
{
	friend class Graphics;
public:
	bool CheckName(const std::string& name);
	static SSprite GetSprite(const std::string& name);
protected:
	Sprite(const STexture& texture, const RECT& sourceRect, const std::string& name);
	static void AddSprite(const STexture& texture, const RECT& sourceRect, const std::string& name);
	static void RemoveSprite(const std::string& name);
	static void ClearSprite();
protected:
	RECT sourceRect;
	STexture source;
	std::string name;

	static std::vector<SSprite> loadedSprite;
};

