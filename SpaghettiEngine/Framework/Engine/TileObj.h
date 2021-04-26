#include <GameObj.h>
#include <TileSpriteRenderer.h>

class TileObj : public GameObj
{
public:
	TileObj(const TileObj& obj);
	TileObj(const std::string& path, const PScene& ownerScene = nullptr);

	void AddTileSpriteRenderer(std::string texturePath, int index, int posX, int posY, int offSetX = 0, int offSetY = 0, int scaleX = 1, int scaleY = 1);
};