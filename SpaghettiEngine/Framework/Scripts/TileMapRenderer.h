#include <fstream>
#include <sstream>
#include <CornException.h>
#include <json.hpp>
#include <Render2DScriptBase.h>
#include <Animation.h>
#include <GameTimer.h>
#include <Setting.h>
#include <Texture.h>

using namespace nlohmann;
using namespace std;

struct AnimationTile
{
	SAnimation animation;
	UINT frame;
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
	double time = 0;

	REGISTER_START(TileMapRenderer);
};