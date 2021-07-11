#pragma once
#if (_WIN32_WINNT >= 0x0602) //_WIN32_WINNT_WIN8
#include <XAudio2.h>
#else
#include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#endif

#include <XAudio2.h>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <vector>
#include <mutex>
#include <atomic>

#include <wrl\client.h>
#include <objbase.h>
#include <cassert>

class SoundSystem
{
public:
	class Error : public std::runtime_error
	{
	public:
		Error(std::string s);
	};
	class FileError : public Error
	{
	public:
		FileError(std::string s);
	};
public:
	class Channel
	{
	private:
		class VoiceCallback : public IXAudio2VoiceCallback
		{
		public:
			void STDMETHODCALLTYPE OnStreamEnd() override;
			void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override;
			void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) override;
			void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override;
			void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override;
			void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override;
			void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) override;
		};
	public:
		Channel(SoundSystem& sys);
		Channel(const Channel&) = delete;
		~Channel();

		void PlaySoundBuffer(class Sound& s, float freqMod, float vol);
		void ChangeVolume(float vol);
		void Stop();
		void Pause();
		void Continue();
	private:
		XAUDIO2_BUFFER xaBuffer;
		IXAudio2SourceVoice* pSource = nullptr;
		// does this need to be synchronized?
		// (no--no overlap of callback thread and main thread here)
		class Sound* pSound = nullptr;
	};
public:
	SoundSystem(const SoundSystem&) = delete;
	static SoundSystem& Get();
	static WAVEFORMATEX& GetFormat();
	void PlaySoundBuffer(class Sound& s, float freqMod, float vol);
private:
	SoundSystem();
	void DeactivateChannel(Channel& channel);
private:
	Microsoft::WRL::ComPtr<IXAudio2> pEngine;
	IXAudio2MasteringVoice* pMaster = nullptr;
	WAVEFORMATEX format;
	const int nChannels = 64;
	std::mutex mutex;
	std::vector<std::unique_ptr<Channel>> idleChannelPtrs;
	std::vector<std::unique_ptr<Channel>> activeChannelPtrs;
};

class Sound
{
	friend SoundSystem::Channel;
public:
	Sound(const std::wstring& fileName);
	Sound(const Sound&) = delete;
	Sound(Sound&& donor);

	void Play(float freqMod, float vol);
	void Continue();
	void Pause();
	void Stop();
	void ChangeVolume(float vol);

	~Sound();
private:
	void RemoveChannel(SoundSystem::Channel& channel);
	void AddChannel(SoundSystem::Channel& channel);
private:
	UINT32 nBytes = 0;
	std::unique_ptr<BYTE[]> pData;
	std::mutex mutex;
	std::vector<SoundSystem::Channel*> activeChannelPtrs;
};