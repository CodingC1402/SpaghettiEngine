#include "Sprite.h"

PTexture& Sprite::GetSource()
{
	return source;
}

int Sprite::GetWidth()
{
	return srcRect.right - srcRect.left;
}

int Sprite::GetHeight()
{
	return srcRect.bottom - srcRect.top;
}

RECT& Sprite::GetSourceRect()
{
	return srcRect;
}

Sprite::Sprite(const PTexture& source, const int& x, const int& y, const int& w, const int& h)
{
	srcRect.left = x;
	srcRect.right = y;
	srcRect.right = srcRect.left + w;
	srcRect.bottom = srcRect.top + h;

	this->source = source;
}

Sprite::Sprite(const PTexture& source, const Size& size, const Point& position)
{
	srcRect.left = position.x;
	srcRect.right = position.y;
	srcRect.right = srcRect.left + size.width;
	srcRect.bottom = srcRect.top + size.height;

	this->source = source;
}
