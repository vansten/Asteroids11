#include "AudioClip.h"

#include <fstream>

#include "Core/Engine.h"

AudioClip::AudioClip(const std::string& wavFileName) : _wavFileName(wavFileName), _sound(nullptr)
{

}

AudioClip::~AudioClip()
{

}

bool AudioClip::Initialize()
{
	FMOD::System& fmodSystem = Engine::GetInstance()->GetAudio().GetFMODSystem();
	FMOD_RESULT result = fmodSystem.createSound(_wavFileName.c_str(), FMOD_3D, 0, &_sound);
	if(result != FMOD_OK)
	{
		printf("Error creating audio clip! Error: %s\n", FMOD_ErrorString(result));
		return false;
	}

	result = _sound->setMode(FMOD_LOOP_NORMAL);
	if(result != FMOD_OK)
	{
		printf("Error creating audio clip! Error: %s\n", FMOD_ErrorString(result));
		return false;
	}

	return true;
}
