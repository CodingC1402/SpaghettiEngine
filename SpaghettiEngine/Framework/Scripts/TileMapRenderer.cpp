#include "TileMapRenderer.h"

REGISTER_FINISH(TileMapRenderer);

void NormalTile::Load(const int& index, Texture* texture, const json& data)
{
	texture->GetSprite(&sprite, index - 1);
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
	animation->Advance(&frame, &time);
}

void AnimatedTile::Load(const int& index, Texture* texture, const json& data)
{
	if (index == -2)
		int i = index;
	animation = Animation::GetAnimation(data["Animations"][abs(index + 1)].get<string>());
}

void AnimatedTile::Draw(SpriteHandler handler, Texture* texture, const Vector3& position)
{
	const SSprite currentSprite = animation->GetSpriteOfFrame(&frame);
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

TileMapRenderer::TileMapRenderer() : width(0), height(0)
{
	name = TYPE_NAME(SpriteRenderer);
}

void TileMapRenderer::Update()
{
	
	for (const auto& tile : animatedTiles)
		tile->Update();
	Render2DScriptBase::Update();
}

void TileMapRenderer::Load(const string* inputArg)
{
	ifstream file(inputArg->c_str());
	if (!file.is_open())
	{
		wostringstream os;
		os << L"File ";
		os << inputArg->c_str();
		os << L" Doesn't exist";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		Texture::GetTexture(&texture,jsonFile["Texture"]);

		width = jsonFile["Width"].get<int>();
		height = jsonFile["Height"].get<int>();
		tileWidth = jsonFile["TileWidth"].get<int>();
		tileHeight = jsonFile["TileHeight"].get<int>();
		
		for (PTile newTile; int index : jsonFile["Data"])
		{
			if (index > 0)
				newTile = new NormalTile;
			else if (index < 0)
				newTile = new AnimatedTile();
			else
				newTile = new Tile();
			newTile->Load(index, texture.get(), jsonFile);
			tiles.push_back(newTile);
		}
		
		file.close();
	}
	catch (...)
	{
		wostringstream os;
		os << L"File ";
		os << inputArg->c_str();
		os << L" doesn't have the right format";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
}

void TileMapRenderer::Draw(SpriteHandler handler, PCamera camera)
{
	Matrix transform = camera->GetMatrix(GetWorldMatrix());
	if (Setting::IsWorldPointPixelPerfect())
	{
		transform._41 = std::round(transform._41);
		transform._42 = std::round(transform._42);
	}
	handler->SetTransform(&transform);

	int k = 0;

	for (int px, py, x, y; const auto& tile : tiles)
	{
		x = k % width;
		y = (k / width);
		k++;
		px = x * tileWidth;
		py = y * tileHeight;
		Vector3 pos(px, py, 0);
		tile->Draw(handler, texture.get(), pos);
	}
}

TileMapRenderer::~TileMapRenderer()
{
	for (const auto& tile : tiles)
		delete tile;
}
