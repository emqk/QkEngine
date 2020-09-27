#include "AudioManager.h"

irrklang::ISoundEngine* AudioManager::SoundEngine = irrklang::createIrrKlangDevice();

void AudioManager::PlayAudioClip(const char* path)
{
	SoundEngine->play2D("350899__cabled-mess__jump-c-09.wav");
}
