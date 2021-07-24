#include "ResourceContainer.h"
#include "Sprite.h"
#include "Macros.h"
#include <map>

CLASS_FORWARD_DECLARATION(TextMap);

class TextMap : Resource
{
public:
	void Load(const std::string& path) override;
	[[nodiscard]] const SSprite&	GetSprite(char letter);
	[[nodiscard]] const float&		GetWidth() const noexcept;
	[[nodiscard]] const float&		GetHeight() const noexcept;
	[[nodiscard]] const float&		GetSpacing() const noexcept;
private:
	std::map<char, SSprite> _decoder;
	float _width = 0.0f;
	float _height = 0.0f;
	float _spacing = 0.0f;

	static constexpr auto _widthField = "Width";
	static constexpr auto _heightField = "Height";
	static constexpr auto _spacingtField = "Spacing";

	static constexpr auto _charField = "Char";
	static constexpr auto _textureField = "Texture";
	static constexpr auto _indexField = "Index";
	static constexpr auto _mapField = "Map";
};

class TextMapContainer : public Container<TextMap>
{
	friend class SceneManager;
public:
	TextMapContainer();
};