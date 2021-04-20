#pragma once
#include "Plane2D.h"
#include "Texture.h"
#include "CornDirectX.h"
#include <memory>
#include <string>

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

typedef class Texture* PTexture;

using Plane2D::Size;
using Plane2D::Point;

class Sprite
{
	friend class Texture;
public:
	PTexture& GetSource();
	int GetWidth();
	int GetHeight();
	const Vector3* GetCenter();
	const RECT* GetSourceRect();
protected:
	Sprite(const PTexture& source, const int& x, const int& y, const int& w, const int& h);
	Sprite(const PTexture& source, const Size& size, const Point& position);
protected:
	RECT srcRect;
	PTexture source;
	Vector3 center;
};

