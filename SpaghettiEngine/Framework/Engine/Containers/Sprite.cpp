#include "Sprite.h"
#include "ExMath.h"

PTexture Sprite::GetSource() const
{
	return source;
}

int Sprite::GetWidth() const
{
	return srcRect.right - srcRect.left;
}

int Sprite::GetHeight() const
{
	return srcRect.bottom - srcRect.top;
}

Vector3 Sprite::GetCenter() const
{
	return center;
}

RECT Sprite::GetSourceRect() const
{
	return srcRect;
}

Sprite::Sprite(const PTexture& source, const int& x, const int& y, const int& w, const int& h)
{
	srcRect.left = x;
	srcRect.top = y;
	srcRect.right = srcRect.left + w;
	srcRect.bottom = srcRect.top + h;

	center.x = std::floor(CLib::ToFloat(w) / 2.0f);
	center.y = std::floor(CLib::ToFloat(h) / 2.0f);
	center.z = 0;

	this->source = source;
}

Sprite::Sprite(const PTexture& source, const Size& size, const Point& position)
{
	srcRect.left = position.x;
	srcRect.top = position.y;
	srcRect.right = srcRect.left + size.width;
	srcRect.bottom = srcRect.top + size.height;

	center.x = std::floor(CLib::ToFloat(size.width) / 2.0f);
	center.y = std::floor(CLib::ToFloat(size.height) / 2.0f);
	center.z = 0;

	this->source = source;
}
