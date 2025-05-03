#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>

class SoundManager
{
public:
	enum eSoundType
	{
		SOUND_MOVE,
		SOUND_ROTATE,
		SOUND_HARDDROP,
		SOUND_HOLD,
		SOUND_SINGLE,
		SOUND_DOUBLE,
		SOUND_TRIPLE,
		SOUND_TETRIS,
		SOUND_TSPIN,
		SOUND_PERFECTCLEAR,
		SOUND_COMBO1,
		SOUND_COMBO2,
		SOUND_COMBO3,
		SOUND_COMBO4,
		SOUND_COMBO5,
		SOUND_COMBO6,
		SOUND_COMBO7,
		SOUND_BACKTOBACK,
		SOUND_GAMEOVER,
		SOUND_BGM_TITLE,
		SOUND_BGM_PLAY,
		SOUND_BGM_RANKING,
		SOUND_CLICK,
		SOUND_COUNT
	};

	SoundManager();
	~SoundManager();

	bool Init();
	void Update();
	void Release();

	bool LoadSound(eSoundType type, const char* filename, bool loop);

	void PlayBGM(eSoundType type, float volume);
	void PlaySFX(eSoundType type, float volume);

	void SetSFXVolume(float volume);

	FMOD::Channel* GetPrevChannel() const;

	void StopBGM();
	void StopAllSFX();
private:
	FMOD::System* m_system;

	FMOD::Channel* m_bgm;
	FMOD::ChannelGroup* m_sfxGroup;

	static const int MAX_SFX_CHANNELS = 20;
	FMOD::Channel* m_sfxChannels[MAX_SFX_CHANNELS];
	int m_nextChannel;

	FMOD::Sound* m_sounds[SOUND_COUNT];
};

