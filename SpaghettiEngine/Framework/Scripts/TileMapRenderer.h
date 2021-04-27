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

class AnimationTile
{
public:
	AnimationTile():animation(nullptr), frame(0), sprite(nullptr) {}
	SAnimation animation;
	SSprite sprite;
	UINT frame;
	double time;
};

class TileMapRenderer : public Render2DScriptBase
{
public:
	TileMapRenderer();

	void Update() override;
	void Load(const string* inputArg) override;
	void Draw(SpriteHandler handler, PCamera camera) override;

private:
	STexture texture;
	int width;
	int height;
	vector<int> data;
	vector<AnimationTile*> animations;
	
	UINT frame = 0;

	REGISTER_START(TileMapRenderer);
};