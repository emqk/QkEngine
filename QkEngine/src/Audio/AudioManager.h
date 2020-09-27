#pragma once
#include <irrKlang.h>

class AudioManager
{
public:
	AudioManager() = delete;
	~AudioManager() = delete;

	static void PlayAudioClip(const char* path);

private:
	static irrklang::ISoundEngine* SoundEngine;

};