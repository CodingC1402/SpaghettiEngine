#pragma once
#include "Texture.h"
#include "Graphics.h"

typedef class Tile
{
public:
    virtual void Load(Texture* texture, std::vector<unsigned> sprites, float delay) {}
    virtual void Draw(const Vector3& position) {}
    virtual ~Tile() = default;
protected:
}*PTile;

typedef class NormalTile : public Tile
{
    virtual void Load(Texture* texture, std::vector<unsigned> sprites, float delay) override;
    virtual void Draw(const Vector3& position) override;
protected:
    SSprite sprite;
}*PNormalTile;

typedef class AnimatedTile : public Tile
{

public:
    virtual void Load(Texture* texture, std::vector<unsigned> sprites, float delay) override;
    virtual void Draw(const Vector3& position) override;
protected:
    std::vector<SSprite> _sprites;
    SSprite _currentSprite;
    float _delay = 0;
    float _currentTime = 0;
}*PAnimatedTile;
typedef std::shared_ptr<Tile> STile;
typedef std::weak_ptr<Tile> WTile;

class TileSet : public Resource
{
public:
	WTile GetTile(int index);
    void Load(const std::string& path) override;
protected:
	std::vector<STile> _tiles;
	STexture _texture;
};

class TileSetContainer : public Container<TileSet>
{
	friend class SceneManager;
public:
	TileSetContainer();
};