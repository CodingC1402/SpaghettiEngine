#pragma once
#include "json.hpp"
#include "CornException.h"
#include "ResourceContainer.h"

#include <string>
#include <memory>
#include <map>

typedef class Texture* PTexture;
typedef std::shared_ptr<Texture> STexture;
typedef std::unique_ptr<Texture> UTexture;

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

class Texture : public Resource
{
public:
	Texture();
	~Texture() override;
	
	void Load(const std::string& path) override;
	[[nodiscard]] bool IsResourceUnused() const override;
	[[nodiscard]] PImage GetImage() const;
	[[nodiscard]] SSprite GetSprite(const unsigned int& index) noexcept;
	[[nodiscard]] std::vector<SSprite> GetAllSprites() noexcept;
	[[nodiscard]] unsigned GetSpritesNumber() noexcept;
protected:
	std::vector<SSprite>sprites;
	PImage image;
};

class TextureContainer : public Container<Texture>
{
	friend class SceneManager;
public:
	TextureContainer();
};