#pragma once
#include "Macros.h"

typedef unsigned char Byte;
typedef unsigned long Int32;

struct Color
{
public:
	Color() = default;
	Color(Int32 color);
	Color(Byte r, Byte g, Byte b, Byte a = 255);
	Int32 GetDXColor() const;
	operator Int32() const;

	PROPERTY(Byte, alpha);
	GET(alpha) const {
		return (_color >> _alphaMask) & _mask;
	}
	SET(alpha) {
		_color |= (_mask << _alphaMask); 
		_color &= (static_cast<Int32>(value & _mask) << _alphaMask);
	}

	PROPERTY(Byte, red);
	GET(red) const {
		return (_color >> _redMask) & _mask;
	}
	SET(red) {
		_color |= (_mask << _redMask);
		_color &= (static_cast<Int32>(value & _mask) << _redMask);
	}

	PROPERTY(Byte, green);
	GET(green) const {
		return (_color >> _greenMask) & _mask;
	}
	SET(green) {
		_color |= (_mask << _greenMask);
		_color &= (static_cast<Int32>(value & _greenMask) << _greenMask);
	}

	PROPERTY(Byte, blue);
	GET(blue) const {
		return (_color >> _blueMask) & _mask;
	}
	SET(blue) {
		_color |= (_mask << _blueMask);
		_color &= (static_cast<Int32>(value & _blueMask) << _blueMask);
	}
protected:
	static constexpr Byte _alphaMask = 24;
	static constexpr Byte _redMask = 16;
	static constexpr Byte _greenMask = 8;
	static constexpr Byte _blueMask = 0;
	static constexpr Int32 _mask = 0x000000FF;
	Int32 _color = 0;
};

#define XRGB Color
#define ARGB(alpha, r, g, b) Color(alpha, r, g, b);

// decrease drag time but increase memory use so use wisely :v,
// jk like I know shit :v
#define BLACK	XRGB(0, 0, 0)
#define WHITE	XRGB(255, 255, 255)
#define MAGENTA XRGB(255, 0, 255)
#define BLUE	XRGB(0, 0, 255)
#define GREEN	XRGB(0, 255, 0)
#define RED		XRGB(255, 0, 0)