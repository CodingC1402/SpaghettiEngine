#pragma once

namespace UIField
{
	class Component
	{
	public:
		static constexpr auto _type			= "Type";
		static constexpr auto _name			= "Name";
		static constexpr auto _color		= "Color";
		static constexpr auto _position		= "Position";
		static constexpr auto _components	= "Components";
	};

	class RectangleUI
	{
	public:
		static constexpr auto _texture	= "Texture";
		static constexpr auto _index	= "Index";
	};

	class UIString
	{
	public:
		static constexpr auto _text = "Text";
		static constexpr auto _textMap = "TextMap";
	};
}