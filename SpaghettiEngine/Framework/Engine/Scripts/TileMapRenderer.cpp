#include "TileMapRenderer.h"
#include "json.hpp"
#include "GameTimer.h"
#include "Animation.h"
#include "SMath.h"
#include "LoadingJson.h"
#include "Graphics.h"
#include "SMath.h"

#include <fstream>
#include <algorithm>
#include <Setting.h>

using namespace nlohmann;
using namespace std;

REGISTER_FINISH(TileMapRenderer, Render2DScriptBase),
	width(0),
	height(0),
	tileWidth(0),
	tileHeight(0)
{}

void TileMapRenderer::Load(nlohmann::json& inputObject)
{
	json copy = inputObject;
	std::string tileMapFilePath;

	constexpr const char* TileMapPath = "TileMapPath";
	try
	{
		tileMapFilePath = copy[TileMapPath].get<std::string>();
	}
	catch (const std::exception&)
	{
		std::wostringstream os;
		os << "\n[Error field] " << TileMapPath;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	ifstream file(tileMapFilePath);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't exist" << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		width = jsonFile["Width"].get<int>();
		height = jsonFile["Height"].get<int>();
		tileWidth = jsonFile["TileWidth"].get<int>();
		tileHeight = jsonFile["TileHeight"].get<int>();
		_pixelWidth = static_cast<float>(width * tileWidth);
		_pixelHeight = static_cast<float>(height * tileHeight);
		
		_tileSet = TileSetContainer::GetInstance()->GetResource(jsonFile[LoadingJson::Field::idField]);

		int row = 0;
		int col = 0;
		_tiles.reserve(height);
		_tiles.emplace_back(std::vector<WTile>());
		_tiles[0].reserve(width);
		for (WTile tile; int index : jsonFile["Data"])
		{
			if (col == width)
			{
				_tiles.emplace_back(std::vector<WTile>());
				col = 0;
				row++;
				_tiles[row].reserve(width);
			}

			if (index > 0) // If > 0 then it's a animated tile or a normal tile
			{
				tile = _tileSet->GetTile(index - 1);
				_tiles[row].emplace_back(tile);
			}
			else // If = 0 then it's empty
				_tiles[row].emplace_back(WTile());
			col++;
		}

		file.close();
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't have the right format" << std::endl;
		os << "[Exception] " << e.what() << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	Render2DScriptBase::Load(inputObject);
}

void TileMapRenderer::Draw(PCamera camera)
{
	const float halfHeightPx = (SMath::ToFloat(height) / 2.0f) * SMath::ToFloat(tileHeight);
	const float halfWidthPx = (SMath::ToFloat(width) / 2.0f) * SMath::ToFloat(tileWidth);

	//Set tiles map transform
	Matrix4 transform = GetWorldMatrix();
	transform._42 += halfHeightPx;
	transform._41 -= halfWidthPx;
	transform = camera->GetMatrix(transform);
	Graphics::SetSpriteTransform(transform);

	//get screen height and width then divide them by 2;
	Size viewPort = Setting::GetHalfResolution();
	Matrix4 tileMapMatrix = GetWorldMatrix().Inverse();

	tileMapMatrix = camera->GetWorldMatrix() * tileMapMatrix;

	Vector3 points[4];
	points[0] = { -static_cast<float>(viewPort.width), -static_cast<float>(viewPort.height), 0 };
	points[1] = { -static_cast<float>(viewPort.width),  static_cast<float>(viewPort.height), 0 };
	points[2] = { static_cast<float>(viewPort.width), -static_cast<float>(viewPort.height), 0 };
	points[3] = { static_cast<float>(viewPort.width),  static_cast<float>(viewPort.height), 0 };

	SMath::TransformVector3(points[0], points[0], tileMapMatrix);
	SMath::TransformVector3(points[1], points[1], tileMapMatrix);
	SMath::TransformVector3(points[2], points[2], tileMapMatrix);
	SMath::TransformVector3(points[3], points[3], tileMapMatrix);

	float maxX = points[0].x, maxY = points[0].y, minX = points[0].x, minY = points[0].y;
	for (const auto& point : points)
	{
		if (point.x > maxX)
			maxX = point.x;
		else if (point.x < minX)
			minX = point.x;

		if (point.y > maxY)
			maxY = point.y;
		else if (point.y < minY)
			minY = point.y;
	}

	std::swap(maxY, minY);
	maxY = _pixelHeight - (maxY + _pixelHeight / 2.0f);
	minY = _pixelHeight - (minY + _pixelHeight / 2.0f);
	maxX = maxX + SMath::ToFloat(_pixelWidth) / 2.0f;
	minX = minX + SMath::ToFloat(_pixelWidth) / 2.0f;
	int startRow = SMath::ceili(minY / SMath::ToFloat(tileWidth)) - 1;
	int endRow = SMath::ceili(maxY / SMath::ToFloat(tileWidth)) + 1;
	int startCol = SMath::ceili(minX / SMath::ToFloat(tileWidth)) - 1;
	int endCol = SMath::ceili(maxX / SMath::ToFloat(tileWidth)) + 1;

	startRow = startRow < 0 ? 0 : startRow;
	endRow = endRow < height ? endRow : height;
	startCol = startCol < 0 ? 0 : startCol;
	endCol = endCol < width ? endCol : width;

	Vector3 position;
	position.z = 0;
	for (int row = startRow; row < endRow; row++)
	{
		position.y = static_cast<float>(row * tileHeight);
		for (int col = startCol; col < endCol; col++)
		{
			if (!_tiles[row][col].expired())
			{
				position.x = static_cast<float>(col * tileWidth);
				_tiles[row][col].lock()->Draw(position);
			}
		}
	}
}

PScriptBase TileMapRenderer::Clone() const
{
	auto clone = dynamic_cast<TileMapRenderer*>(Render2DScriptBase::Clone());

	clone->_pixelHeight = _pixelHeight;
	clone->_pixelWidth = _pixelWidth;
	clone->_tiles = _tiles;
	clone->_tileSet = _tileSet;

	return clone;
}

TileMapRenderer::~TileMapRenderer()
{
	_tiles.clear();
}