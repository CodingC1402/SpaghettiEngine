#include <SpriteRenderer.h>

class TileSpriteRenderer : public SpriteRenderer
{
	friend class TileObj;
public:
	TileSpriteRenderer();

	void LoadTile(const std::string& texturePath, int index, int offSetX = 0, int offSetY = 0, int scaleX = 1, int scaleY = 1);
	int GetSrcWidth() const;
	int GetSrcHeight() const;

private:
	REGISTER_START(TileSpriteRenderer);
};