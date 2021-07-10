#pragma once
#include <memory>
#include "ResourceContainer.h"
#include "Sound.h"

typedef class Audio* PAudio;
typedef std::shared_ptr<Audio> SAudio;
typedef std::unique_ptr<Audio> UAudio;

class Audio : public Resource
{
	friend class AudioContainer;
public:
	Audio();

	[[nodiscard]] size_t GetNumberOfSounds() const noexcept;
	void Load(const std::string& path) override;
protected:
	float masterVolume = 1.0f;
	std::vector<Sound> _sounds;
	bool isLoop;
};

class AudioContainer : public Container<Audio>
{
	friend class SceneManager;
public:
	AudioContainer();
};