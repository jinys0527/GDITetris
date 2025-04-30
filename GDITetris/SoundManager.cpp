#include "SoundManager.h"
#include "iostream"

SoundManager::SoundManager() 
	: m_system(nullptr)
	, m_bgm(nullptr)
	, m_sfxGroup(nullptr)
	, m_nextChannel(0)
{
	for (int i = 0; i < MAX_SFX_CHANNELS; i++)
	{
		m_sfxChannels[i] = nullptr;
	}

	for (int i = 0; i < SOUND_COUNT; i++)
	{
		m_sounds[i] = nullptr;
	}
}

SoundManager::~SoundManager()
{
	Release();
}

bool SoundManager::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD 시스템 생성 실패 " << FMOD_ErrorString(result);
		return false;
	}

	result = m_system->init(20, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD 초기화 실패 " << FMOD_ErrorString(result);
		return false;
	}

	result = m_system->createChannelGroup("SFX", &m_sfxGroup);

	return true;
}

void SoundManager::Update()
{
	if (m_system)
	{
		m_system->update();
	}
}

void SoundManager::Release()
{
	for (int i = 0; i < SOUND_COUNT; i++)
	{
		if (m_sounds[i])
		{
			m_sounds[i]->release();
			m_sounds[i] = nullptr;
		}
	}

	if (m_sfxGroup)
	{
		m_sfxGroup->release();
		m_sfxGroup = nullptr;
	}

	if (m_system)
	{
		m_system->close();
		m_system->release();
		m_system = nullptr;
	}
}

bool SoundManager::LoadSound(eSoundType type, LPCWSTR filename, bool loop)
{
	if (!m_system || type >= SOUND_COUNT)
	{
		return false;
	}

	if (m_sounds[type])
	{
		m_sounds[type]->release();
		m_sounds[type] = nullptr;
	}

	FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	FMOD_RESULT result = m_system->createSound(reinterpret_cast<const char*>(filename), mode, nullptr, &m_sounds[type]);

	if (result != FMOD_OK)
	{
		std::cout << "사운드 로드 실패 " << type << FMOD_ErrorString(result);
		return false;
	}

	return true;
}

void SoundManager::PlayBGM(float volume)
{
	if (!m_system || !m_sounds[SOUND_BGM])
	{
		return;
	}

	bool isPlaying = false;
	if (m_bgm)
	{
		m_bgm->isPlaying(&isPlaying);
	}

	if (!isPlaying)
	{
		m_system->playSound(m_sounds[SOUND_BGM], nullptr, false, &m_bgm);
		if (m_bgm)
		{
			m_bgm->setVolume(volume);
		}
	}
}

void SoundManager::PlaySFX(eSoundType type, float volume)
{
	if (!m_system || !m_sounds[type] || type >= SOUND_COUNT)
	{
		return;
	}

	if (type == SOUND_BGM)
	{
		return;
	}

	int channelIndex = m_nextChannel;
	m_nextChannel = (m_nextChannel + 1) % MAX_SFX_CHANNELS;

	m_system->playSound(m_sounds[type], m_sfxGroup, false, &m_sfxChannels[channelIndex]);

	if (m_sfxChannels[channelIndex])
	{
		m_sfxChannels[channelIndex]->setVolume(volume);
	}
}

void SoundManager::SetSFXVolume(float volume)
{
	if (m_sfxGroup)
	{
		m_sfxGroup->setVolume(volume);
	}
}

void SoundManager::StopBGM()
{
	if (m_bgm)
	{
		m_bgm->stop();
	}
}

void SoundManager::StopAllSFX()
{
	if (m_sfxGroup)
	{
		m_sfxGroup->stop();
	}
}
