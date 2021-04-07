#pragma once
#include "Plane2D.h"
#include "Texture.h"
#include <memory>
#include <string>

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

class Texture;
typedef std::shared_ptr<Texture> STexture;

using Plane2D::Size;
using Plane2D::Point;

class Sprite
{
	friend class Texture;
public:
	STexture& GetSource();
	RECT& GetSourceRect();
protected:
	Sprite(const STexture& source, const Size& size, const Point& position);
protected:
	RECT srcRect;
	STexture source;
};

