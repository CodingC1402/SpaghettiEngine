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

Color::Color(nlohmann::json& input)
{
	alpha	= input[0].get<Byte>();
	red		= input[1].get<Byte>();
	green	= input[2].get<Byte>();
	blue	= input[3].get<Byte>();
}

Int32 Color::GetDXColor() const
{
	return _color;
}

Color::operator Int32() const
{
	return _color;
}
