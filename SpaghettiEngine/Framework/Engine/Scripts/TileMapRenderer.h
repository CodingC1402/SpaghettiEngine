#pragma once

#include "TileSet.h"
#include <Render2DScriptBase.h>
#include <vector>

typedef class TileMapRenderer* PTileMapRenderer;

class TileMapRenderer : public Render2DScriptBase
{
public:
    TileMapRenderer(PScene owner);

    void Load(nlohmann::json& inputObject) override;
    void Draw(PCamera camera) override;

    PScriptBase Clone() const override;
    ~TileMapRenderer() override;
protected:
    float _pixelHeight;
    float _pixelWidth;
    int width;
    int height;
    int tileWidth;
    int tileHeight;

    std::shared_ptr<TileSet> _tileSet;
    std::vector<std::vector<WTile>> _tiles;

    REGISTER_START(TileMapRenderer);
};