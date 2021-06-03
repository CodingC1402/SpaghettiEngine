#include "TileSet.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include "Sprite.h"
#include "GameTimer.h"
#include "SMath.h"
#include "CornException.h"
#include "LoadingJson.h"

CONTAINER_REGISTER(TileSetContainer, TileSet);
using namespace nlohmann;

void NormalTile::Load(Texture* texture, std::vector<unsigned> sprites, float fps)
{
	sprite = texture->GetSprite(sprites[0]);
}

void NormalTile::Draw(const Vector3& position)
{
	Graphics::DrawSprite(sprite, sprite->GetCenter(), position);
}

void AnimatedTile::Load(Texture* texture, std::vector<unsigned> sprites, float fps)
{
	if (fps <= 0.0001)
	{
		std::wostringstream os;
		os << "[Exception] the fps is too low";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
	_delay = 1 / fps;
	for (int i = 0; i < sprites.size(); i++)
		_sprites.push_back(texture->GetSprite(sprites[i]));
	_currentSprite = _sprites.front();
}

void AnimatedTile::Draw(const Vector3& position)
{
	if (_currentTime != GameTimer::GetGameTime())
	{
		_currentTime = GameTimer::GetGameTime();
		_currentSprite = _sprites[SMath::modulo<unsigned>(static_cast<unsigned>(GameTimer::GetGameTime() / _delay), static_cast<unsigned>(_sprites.size()))];
	}
	Vector3 center = _currentSprite->GetCenter();
	Graphics::DrawSprite(_currentSprite, _currentSprite->GetCenter(), position);
}

TileSetContainer::TileSetContainer()
{
	_name = RESOURCE_NAME(TileSet);
	LoadEntries(SystemPath::TileSetPath);
}

WTile TileSet::GetTile(int index)
{
	if (index < 0 || index > _tiles.size())
	{
		std::wostringstream os;
		os << "[Exceptions] You are trying to get a tile with a non exist id";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
	else
	{
		return _tiles[index];
	}
}

void TileSet::Load(const std::string& path)
{
	using namespace LoadingJson;
	using namespace nlohmann;

	std::ifstream file(path);
	if (!file.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Animation);
	}

	int fieldTracker = 0;
	try
	{
		json jsonFile;
		file >> jsonFile;

		auto textureID = jsonFile[Field::textureField].get<CULL>();
		fieldTracker++;
		STexture texture = TextureContainer::GetInstance()->GetResource(textureID);

		std::vector<unsigned> id = { 0 };
		for (STile newTile; id[0] < texture->GetSpritesNumber(); id[0]++)
		{
			newTile.reset(new NormalTile());
			newTile->Load(texture.get(), id, 1);
			_tiles.push_back(newTile);
		}
		id.clear();

		fieldTracker++;
		for (STile newTile; auto & anim : jsonFile[Field::animationsField])
		{
			newTile.reset(new AnimatedTile());
			id = anim[Field::spritesField].get<std::vector<unsigned>>();
			newTile->Load(texture.get(), id, anim[Field::fpsField].get<float>());
			_tiles[anim[Field::idField].get<unsigned>()] = newTile;
		}
	}
	catch (...)
	{
		std::ostringstream os;
		os << "[Field] ";
		switch (fieldTracker)
		{
		case 0:
			os << Field::textureField;
			break;
		case 1:
			os << Field::spritesField;
			break;
		case 2:
			os << Field::animationsField;
			break;
		}
		os << std::endl;

		os << "[Exception] Field doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Animation);
	}
}
