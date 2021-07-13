#include <assert.h>
#include "Sound.h"

SoundSystem* SoundSystem::__instance = nullptr;

SoundSystem::Error::Error(std::string s) : runtime_error(std::string("SoundSystem::Error: ") + s)
{
}

SoundSystem::FileError::FileError(std::string s) : Error(std::string("SoundSystem::FileError: ") + s)
{
}

SoundSystem* SoundSystem::Get() noexcept
{
	if (!__instance)
		__instance = new SoundSystem();
	return __instance;
}

WAVEFORMATEX& SoundSystem::GetFormat()
{
	return Get()->format;
}

void SoundSystem::PlaySoundBuffer(Sound& s, float freqMod, float vol)
{
	std::lock_guard<std::mutex> lock(mutex);
	if (idleChannelPtrs.size() > 0)
	{
		activeChannelPtrs.push_back(std::move(idleChannelPtrs.back()));
		idleChannelPtrs.pop_back();
		activeChannelPtrs.back()->PlaySoundBuffer(s, freqMod, vol);
	}
}

SoundSystem::~SoundSystem()
{
	pEngine->Release();
	CoUninitialize();
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
	CoInitializeEx(0, COINIT_MULTITHREADED);
	XAudio2Create(&pEngine);
	pEngine->CreateMasteringVoice(&pMaster);
	for (int i = 0; i < nChannels; i++)
	{
		idleChannelPtrs.push_back(std::make_unique<Channel>(*this));
	}
}

void SoundSystem::DeactivateChannel(Channel& channel)
{
	std::lock_guard<std::mutex> lock(mutex);
	auto i = std::find_if(activeChannelPtrs.begin(), activeChannelPtrs.end(),
		[&channel](const std::unique_ptr<Channel>& pChan) -> bool
		{
			return &channel == pChan.get();
		});
	idleChannelPtrs.push_back(std::move(*i));
	activeChannelPtrs.erase(i);
}

void SoundSystem::Channel::VoiceCallback::OnStreamEnd()
{
}

void SoundSystem::Channel::VoiceCallback::OnVoiceProcessingPassEnd()
{
}

void SoundSystem::Channel::VoiceCallback::OnVoiceProcessingPassStart(UINT32 SamplesRequired)
{
}

void SoundSystem::Channel::VoiceCallback::OnBufferEnd(void* pBufferContext)
{
	Channel& chan = *(Channel*)pBufferContext;
	chan.Stop();
	chan.pSound->RemoveChannel(chan);
	chan.pSound = nullptr;
	SoundSystem::Get()->DeactivateChannel(chan);
}

void SoundSystem::Channel::VoiceCallback::OnBufferStart(void* pBufferContext)
{
}

void SoundSystem::Channel::VoiceCallback::OnLoopEnd(void* pBufferContext)
{
}

void SoundSystem::Channel::VoiceCallback::OnVoiceError(void* pBufferContext, HRESULT Error)
{
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

void SoundSystem::Channel::Resume()
{
	pSource->Start();
}

Sound::Sound(const std::wstring& fileName)
{
	unsigned int fileSize = 0;
	std::unique_ptr<BYTE[]> pFileIn;
	try
	{
		{
			std::ifstream file;
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				file.open(fileName, std::ios::binary);
			}
			catch (std::exception e)
			{
				throw SoundSystem::FileError(
					std::string("error opening file: ") + e.what());
			}

			{
				int fourcc;
				file.read(reinterpret_cast<char*>(&fourcc), 4);
				if (fourcc != 'FFIR')
				{
					throw SoundSystem::FileError("bad fourCC");
				}
			}

			file.read(reinterpret_cast<char*>(&fileSize), 4);
			fileSize += 8; // entry doesn't include the fourcc or itself
			if (fileSize <= 44)
			{
				throw SoundSystem::FileError("file too small");
			}

			file.seekg(0, std::ios::beg);
			pFileIn = std::make_unique<BYTE[]>(fileSize);
			file.read(reinterpret_cast<char*>(pFileIn.get()), fileSize);
		}

		if (*reinterpret_cast<const int*>(&pFileIn[8]) != 'EVAW')
		{
			throw SoundSystem::FileError("format not WAVE");
		}

		//look for 'fmt ' chunk id
		WAVEFORMATEX format;
		bool bFilledFormat = false;
		for (unsigned int i = 12; i < fileSize; )
		{
			if (*reinterpret_cast<const int*>(&pFileIn[i]) == ' tmf')
			{
				memcpy(&format, &pFileIn[i + 8], sizeof(format));
				bFilledFormat = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			i += (*reinterpret_cast<const int*>(&pFileIn[i + 4]) + 9) & 0xFFFFFFFE;
		}
		if (!bFilledFormat)
		{
			throw SoundSystem::FileError("fmt chunk not found");
		}

		// compare format with sound system format
		{
			const WAVEFORMATEX sysFormat = SoundSystem::GetFormat();

			if (format.nChannels != sysFormat.nChannels)
			{
				throw SoundSystem::FileError("bad wave format (nChannels)");
			}
			else if (format.wBitsPerSample != sysFormat.wBitsPerSample)
			{
				throw SoundSystem::FileError("bad wave format (wBitsPerSample)");
			}
			else if (format.nSamplesPerSec != sysFormat.nSamplesPerSec)
			{
				throw SoundSystem::FileError("bad wave format (nSamplesPerSec)");
			}
			else if (format.wFormatTag != sysFormat.wFormatTag)
			{
				throw SoundSystem::FileError("bad wave format (wFormatTag)");
			}
			else if (format.nBlockAlign != sysFormat.nBlockAlign)
			{
				throw SoundSystem::FileError("bad wave format (nBlockAlign)");
			}
			else if (format.nAvgBytesPerSec != sysFormat.nAvgBytesPerSec)
			{
				throw SoundSystem::FileError("bad wave format (nAvgBytesPerSec)");
			}
		}

		//look for 'data' chunk id
		bool bFilledData = false;
		for (unsigned int i = 12; i < fileSize; )
		{
			const int chunkSize = *reinterpret_cast<const int*>(&pFileIn[i + 4]);
			if (*reinterpret_cast<const int*>(&pFileIn[i]) == 'atad')
			{
				pData = std::make_unique<BYTE[]>(chunkSize);
				nBytes = chunkSize;
				memcpy(pData.get(), &pFileIn[i + 8], nBytes);

				bFilledData = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			i += (chunkSize + 9) & 0xFFFFFFFE;
		}
		if (!bFilledData)
		{
			throw SoundSystem::FileError("data chunk not found");
		}
	}
	catch (SoundSystem::Error e)
	{
		throw e;
	}
	catch (std::exception e)
	{
		throw SoundSystem::FileError(e.what());
	}
}

Sound::Sound(Sound&& donor) noexcept : nBytes(donor.nBytes), pData(std::move(donor.pData)), activeChannelPtrs(std::move(donor.activeChannelPtrs))
{
}

void Sound::Play(float freqMod, float vol)
{
	if (activeChannelPtrs.size() != 0)
	{
		Sound::Stop();
	}

	SoundSystem::Get()->PlaySoundBuffer(*this, freqMod, vol);
}

void Sound::Resume()
{
	for (auto pChannel : activeChannelPtrs)
	{
		pChannel->Resume();
	}
}

void Sound::Pause()
{
	for (auto pChannel : activeChannelPtrs)
	{
		pChannel->Pause();
	}
}

void Sound::Stop()
{
	for (auto pChannel : activeChannelPtrs)
	{
		pChannel->Stop();
	}
}

void Sound::ChangeVolume(float vol)
{
	for (auto pChannel : activeChannelPtrs)
	{
		pChannel->ChangeVolume(vol);
	}
}

bool Sound::IsPlaying()
{
	if (activeChannelPtrs.size() != 0)
		return true;

	return false;
}

Sound::~Sound()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		for (auto pChannel : activeChannelPtrs)
		{
			pChannel->Stop();
		}
	}

	bool allChannelsDeactivated = false;
	do
	{
		std::lock_guard<std::mutex> lock(mutex);
		allChannelsDeactivated = activeChannelPtrs.size() == 0;
	} while (!allChannelsDeactivated);
}

void Sound::RemoveChannel(SoundSystem::Channel& channel)
{
	std::lock_guard<std::mutex> lock(mutex);
	activeChannelPtrs.erase(std::find(
		activeChannelPtrs.begin(), activeChannelPtrs.end(), &channel));
}

void Sound::AddChannel(SoundSystem::Channel& channel)
{
	std::lock_guard<std::mutex> lock(mutex);
	activeChannelPtrs.push_back(&channel);
}
