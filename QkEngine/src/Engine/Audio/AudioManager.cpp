#include "AudioManager.h"
#include <iostream>

irrklang::ISoundEngine* AudioManager::soundEngine = irrklang::createIrrKlangDevice();

irrklang::ISoundEngine* AudioManager::GetSoundEngine()
{
	return soundEngine;
}

void AudioManager::PlayAudioClip(irrklang::ISoundSource* audioClip)
{
	if (!audioClip)
		std::cout << "Can't play audio clip - clip is null!\n";

	soundEngine->play2D(audioClip);
}
