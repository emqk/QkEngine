#pragma once
#include <irrKlang.h>

class AudioManager
{
public:
	AudioManager() = delete;
	~AudioManager() = delete;

	static irrklang::ISoundEngine* GetSoundEngine();

	static void PlayAudioClip(irrklang::ISoundSource* audioClip);

private:
	static irrklang::ISoundEngine* soundEngine;

};