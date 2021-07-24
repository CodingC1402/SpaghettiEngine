#include "TextMap.h"
#include "SpaghettiEnginePath.h"
#include "Texture.h"

CONTAINER_REGISTER(TextMapContainer, TextMap);

TextMapContainer::TextMapContainer()
{
	_name = RESOURCE_NAME(Texture);
	LoadEntries(SystemPath::TextMapPath);
}

void TextMap::Load(const std::string& path)
{
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), TextMap);
	}
	json jsonFile;
	file >> jsonFile;

	// Load map
	for (auto& letter : jsonFile[_mapField])
	{
		auto texture = TextureContainer::GetInstance()->GetResource(letter[_textureField].get<CULL>());
		_decoder.emplace(
			letter[_charField].get<std::string>().front(),
			texture->GetSprite(letter[_indexField].get<CULL>())
		);
	}

	_width = jsonFile[_widthField].get<float>();
	_height = jsonFile[_heightField].get<float>();
	_spacing = jsonFile[_spacingtField].get<float>();
}

const SSprite& TextMap::GetSprite(char letter)
{
	return _decoder[letter];
}

const float& TextMap::GetWidth() const noexcept
{
	return _width;
}

const float& TextMap::GetHeight() const noexcept
{
	return _height;
}

const float& TextMap::GetSpacing() const noexcept
{
	return _spacing;
}