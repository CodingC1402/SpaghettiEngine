#include <assert.h>
#include "Sound.h"

SoundSystem& SoundSystem::Get()
{
	static SoundSystem instance;
	return instance;
}

SoundSystem::SoundSystem()
{
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 2;
	format.nSamplesPerSec = 44100;
	format.nAvgBytesPerSec = 176400;
	format.nBlockAlign = 4;
	format.wBitsPerSample = 16;
	format.cbSize = 0;
	XAudio2Create(&pEngine);
	pEngine->CreateMasteringVoice(&pMaster);
	for (int i = 0; i < nChannels; i++)
	{
		idleChannelPtrs.push_back(std::make_unique<Channel>(*this));
	}
}

void SoundSystem::Channel::VoiceCallback::OnBufferEnd(void* pBufferContext)
{
	Channel& chan = *(Channel*)pBufferContext;
	chan.Stop();
	chan.pSound->RemoveChannel(chan);
	chan.pSound = nullptr;
	SoundSystem::Get().DeactivateChannel(chan);
}

SoundSystem::Channel::Channel(SoundSystem& sys)
{
	static VoiceCallback vcb;
	ZeroMemory(&xaBuffer, sizeof(xaBuffer));
	xaBuffer.pContext = this;
	sys.pEngine->CreateSourceVoice(&pSource, &sys.format, 0u, 2.0f, &vcb);
}

SoundSystem::Channel::~Channel()
{
	assert(!pSound);
	if (pSource)
	{
		pSource->DestroyVoice();
		pSource = nullptr;
	}
}

void SoundSystem::Channel::PlaySoundBuffer(Sound& s, float freqMod, float vol)
{
	assert(pSource && !pSound);
	s.AddChannel(*this);
	// callback thread not running yet, so no sync necessary for pSound
	pSound = &s;
	xaBuffer.pAudioData = s.pData.get();
	xaBuffer.AudioBytes = s.nBytes;
	pSource->SubmitSourceBuffer(&xaBuffer, nullptr);
	pSource->SetFrequencyRatio(freqMod);
	pSource->SetVolume(vol);
	pSource->Start();
}

void SoundSystem::Channel::ChangeVolume(float vol)
{
	pSource->SetVolume(vol);
}

void SoundSystem::Channel::Stop()
{
	assert(pSource && pSound);
	pSource->Stop();
	pSource->FlushSourceBuffers();
}

void SoundSystem::Channel::Pause()
{
	pSource->Stop();
}

void SoundSystem::Channel::Continue()
{
	pSource->Start();
}