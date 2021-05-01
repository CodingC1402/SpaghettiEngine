#include "TileMapRenderer.h"
#include "json.hpp"
#include "GameTimer.h"
#include "Animation.h"
#include "ExMath.h"
#include <fstream>
#include <Setting.h>

using namespace nlohmann;
using namespace std;

REGISTER_FINISH(TileMapRenderer);

void NormalTile::Load(const int& index, Texture* texture, const json& data)
{
	sprite = texture->GetSprite(index - 1);
}

void NormalTile::Draw(SpriteHandler handler, Texture* texture, const Vector3& position)
{
	RECT srcRect = sprite->GetSourceRect();
	Vector3 center = sprite->GetCenter();
	handler->Draw(
		texture->GetImage(),
		&srcRect,
		&center,
		&position,
		WHITE
	);
}

void AnimatedTile::Update()
{
	time += GameTimer::GetDeltaTime();
	animation->Advance(frame, time);
}

void AnimatedTile::Load(const int& index, Texture* texture, const json& data)
{
	animation = AnimationContainer::GetInstance()->GetResource(data["Animations"][abs(index + 1)].get<CULL>());
}

void AnimatedTile::Draw(SpriteHandler handler, Texture* texture, const Vector3& position)
{
	const SSprite currentSprite = animation->GetSpriteOfFrame(frame);
	RECT srcRect = currentSprite->GetSourceRect();
	Vector3 center = currentSprite->GetCenter();
	handler->Draw(
		texture->GetImage(),
		&srcRect,
		&center,
		&position,
		WHITE
	);
}

TileMapRenderer::TileMapRenderer() : width(0), height(0), tileWidth(0), tileHeight(0)
{
	name = TYPE_NAME(TileMapRenderer);
}

void TileMapRenderer::Update()
{
	for (const auto& tile : animatedTiles)
		tile->Update();
	Render2DScriptBase::Update();
}

void TileMapRenderer::Load(nlohmann::json& inputObject)
{	
	json copy = inputObject;
	std::string tileMapFilePath;
	
	constexpr const char* TileMapPath = "TileMapPath";
	try
	{
		tileMapFilePath = copy[TileMapPath].get<std::string>();
	}
	catch(const std::exception&)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, std::string("\n[Error field] ") + TileMapPath);
	}
	
	ifstream file(tileMapFilePath);
	if (!file.is_open())
	{
		ostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't exist" << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		texture = TextureContainer::GetInstance()->GetResource(jsonFile["Texture"].get<CULL>());

		width = jsonFile["Width"].get<int>();
		height = jsonFile["Height"].get<int>();
		tileWidth = jsonFile["TileWidth"].get<int>();
		tileHeight = jsonFile["TileHeight"].get<int>();

		int row = 0;
		int col = 0;
		tiles.reserve(height);
		tiles.emplace_back(std::vector<Tile*>());
		tiles[0].reserve(width);
		for (Tile* newTile; int index : jsonFile["Data"])
		{
			if (col == width)
			{
				tiles.emplace_back(std::vector<Tile*>());
				col = 0;
				row++;
				tiles[row].reserve(width);
			}

			if (index > 0)
				newTile = new NormalTile;
			else if (index < 0)
			{
				newTile = new AnimatedTile();
				animatedTiles.push_back(dynamic_cast<AnimatedTile*>(newTile));
			}
			else
				newTile = new Tile();

			newTile->Load(index, texture.get(), jsonFile);
			tiles[row].emplace_back(newTile);

			col++;
		}

		file.close();
	}
	catch (const std::exception& e)
	{
		ostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't have the right format" << std::endl;
		os << "[Exception] " << e.what() << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	Render2DScriptBase::Load(inputObject);
}

void TileMapRenderer::Draw(SpriteHandler handler, PCamera camera)
{
	using CLib::ToFloat;
	using CLib::ToInt;
	using CLib::ceili;
	
	const float halfHeightPx = (ToFloat(height) / 2.0f) * ToFloat(tileHeight);
	const float halfWidthPx = (ToFloat(width) / 2.0f) * ToFloat(tileWidth);

	//Set tiles map transform
	Matrix transform = camera->GetMatrix(GetWorldMatrix());
	transform._42 -= halfHeightPx;
	transform._41 -= halfWidthPx;
	if (Setting::IsWorldPointPixelPerfect())
	{
		transform._41 = std::round(transform._41);
		transform._42 = std::round(transform._42);
	}
	handler->SetTransform(&transform);

	//get screen height and width then divide them by 2;
	Size viewPort = Setting::GetResolution();
	viewPort.height /= 2;
	viewPort.width /= 2;
	
	Vector3 topLeft = owner->GetTransform();
	topLeft.x -= std::round(halfWidthPx);
	topLeft.y += std::round(halfHeightPx);

	//Get camera position based on tiles map
	Vector3 delta = camera->GetTransform();
	delta.x = delta.x - topLeft.x;
	delta.y = topLeft.y - delta.y;
	
	int startRow =	ceili((delta.y - ToFloat(viewPort.height)) / ToFloat(tileWidth)) - 1;
	int endRow =	ceili((delta.y + ToFloat(viewPort.height)) / ToFloat(tileWidth)) + 1;
	int startCol =	ceili((delta.x - ToFloat(viewPort.width)) / ToFloat(tileWidth)) - 1;
	int endCol =	ceili((delta.x + ToFloat(viewPort.width)) / ToFloat(tileWidth)) + 1;
	
	startRow = startRow < 0 ? 0 : startRow;
	endRow = endRow < height ? endRow : height;
	startCol = startCol < 0 ? 0 : startCol;
	endCol = endCol < width ? endCol : width;

	//int startRow = 0;
	//int endRow = height;
	//int startCol = 0;
	//int endCol = width;
	Vector3 position;
	position.z = 0;
	for (int row = startRow; row < endRow; row++)
	{
		position.y = static_cast<float>(row * tileHeight);
		for (int col = startCol; col < endCol; col++)
		{
			position.x = static_cast<float>(col * tileWidth);
			tiles[row][col]->Draw(handler, texture.get(), position);
		}
	}
}

TileMapRenderer::~TileMapRenderer()
{
	for (const auto& row : tiles)
		for (const auto& col : row)
			delete col;
	tiles.clear();
}
