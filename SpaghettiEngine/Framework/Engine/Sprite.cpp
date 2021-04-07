#include "Sprite.h"

STexture& Sprite::GetSource()
{
	return source;
}

RECT& Sprite::GetSourceRect()
{
	return srcRect;
}

Sprite::Sprite(const STexture& source, const Size& size, const Point& position)
{
	srcRect.left = position.x;
	srcRect.right = position.y;
	srcRect.right = srcRect.left + size.width;
	srcRect.bottom = srcRect.top + size.height;

	this->source = source;
}
