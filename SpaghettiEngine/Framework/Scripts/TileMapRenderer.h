#pragma once

#include "Sprite.h"
#include "Animation.h"
#include <Render2DScriptBase.h>
#include <vector>

typedef class TileMapRenderer* PTileMapRenderer;

typedef class Tile
{
public:
    virtual void Load(const int& index, Texture* texture, const nlohmann::json& data = nullptr) {}
    virtual void Draw(Texture* texture, const Vector3& position) {}
    virtual ~Tile() = default;
protected:
}*PTile;
typedef class NormalTile : public Tile
{
    virtual void Load(const int& index, Texture* texture, const nlohmann::json& data = nullptr) override;
    virtual void Draw(Texture* texture, const Vector3& position) override;
protected:
    SSprite sprite;
}*PNormalTile;
typedef class AnimatedTile : public Tile
{
public:
    void Update();
    virtual void Load(const int& index, Texture* texture, const nlohmann::json& data) override;
    virtual void Draw(Texture* texture, const Vector3& position) override;
protected:
    SAnimation animation = nullptr;
    UINT frame = 0;
    float time = 0;
}*PAnimatedTile;

class TileMapRenderer : public Render2DScriptBase
{
public:
    TileMapRenderer(PScene owner);

    void OnUpdate() override;
    void Load(nlohmann::json& inputObject) override;
    void Draw(PCamera camera) override;
    ~TileMapRenderer() override;
protected:
    STexture texture;
    float _pixelHeight;
    float _pixelWidth;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<std::vector<Tile*>> tiles;
    std::list<PAnimatedTile> animatedTiles;

    REGISTER_START(TileMapRenderer);
};