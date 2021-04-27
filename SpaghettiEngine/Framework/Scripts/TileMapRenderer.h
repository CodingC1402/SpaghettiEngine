#include <fstream>
#include <sstream>
#include <CornException.h>
#include <json.hpp>
#include <Render2DScriptBase.h>
#include <Animation.h>
#include <GameTimer.h>
#include <Setting.h>
#include <Texture.h>
#include <Debug.h>

using namespace nlohmann;
using namespace std;

typedef class TileMapRenderer* PTileMapRenderer;

typedef class Tile
{
public:
	virtual void Load(const int& index, Texture* texture, const json& data = nullptr) {}
	virtual void Draw(SpriteHandler handler, Texture* texture, const Vector3& position) {}
	virtual ~Tile() = default;
protected:
	SSprite sprite;
}*PTile;
typedef class NormalTile : public Tile
{
	virtual void Load(const int& index, Texture* texture, const json& data = nullptr) override;
	virtual void Draw(SpriteHandler handler, Texture* texture, const Vector3& position) override;
protected:
	SSprite sprite;
};
typedef class AnimatedTile : public Tile
{
public:
	void Update();
	virtual void Load(const int& index, Texture* texture, const json& data) override;
	virtual void Draw(SpriteHandler handler, Texture* texture, const Vector3& position) override;
protected:
	SAnimation animation = nullptr;
	UINT frame = 0;
	double time = 0;
}*PAnimatedTile;

class TileMapRenderer : public Render2DScriptBase
{
public:
	TileMapRenderer();

	void Update() override;
	void Load(const string* inputArg) override;
	void Draw(SpriteHandler handler, PCamera camera) override;
	~TileMapRenderer() override;
protected:
	STexture texture;
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	vector<PTile> tiles;
	list<PAnimatedTile> animatedTiles;
	
	REGISTER_START(TileMapRenderer);
};