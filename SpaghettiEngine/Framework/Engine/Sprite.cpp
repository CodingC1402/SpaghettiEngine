#include "Sprite.h"

PTexture& Sprite::GetSource()
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

	center.x = (int)w / 2;
	center.y = (int)w / 2;
	center.z = 0;

	this->source = source;
}

Sprite::Sprite(const PTexture& source, const Size& size, const Point& position)
{
	srcRect.left = position.x;
	srcRect.right = position.y;
	srcRect.right = srcRect.left + size.width;
	srcRect.bottom = srcRect.top + size.height;

	center.x = (int)size.width / 2;
	center.y = (int)size.height / 2;
	center.z = 0;

	this->source = source;
}
