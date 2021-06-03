#include "Color.h"
#include "CornDirectX.h"

Color::Color(Int32 color)
{
	_color = color;
}

Color::Color(Byte r, Byte g, Byte b, Byte a)
{
	_color |= static_cast<Int32>((a << _alphaMask) | (r << _redMask) | (g << _greenMask) | (b << _blueMask));
}

Int32 Color::GetDXColor() const
{
	return _color;
}

Color::operator Int32() const
{
	return _color;
}
