#include "Audio.h"

#include <Windows.h>

#include "AudioSource.h"
#include "AudioClip.h"

#undef PlaySound

Audio::Audio() : _fmod(nullptr)
{

}

Audio::~Audio()
{

}

bool Audio::Initialize()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	FMOD_RESULT result = FMOD::System_Create(&_fmod);
	if(result != FMOD_OK)
	{
		printf("Error creating FMOD::System! Error: %s\n", FMOD_ErrorString(result));
		return false;
	}

	result = _fmod->init(512, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		printf("Error initializing FMOD::System! Error: %s\n", FMOD_ErrorString(result));
		return false;
	}

	_fmod->set3DSettings(1.0f, 1.0f, 1.0f);

	return true;
}

void Audio::Shutdown()
{
	if(_fmod)
	{
		_fmod->release();
		_fmod = nullptr;
	}

	CoUninitialize();
}

void Audio::Update(float deltaTime)
{
	_fmod->update();
}

void Audio::PlaySound(AudioSource* clip)
{
	if(clip == nullptr || clip->_clip == nullptr)
	{
		return;
	}

	_fmod->playSound(clip->_clip->_sound, 0, false, &clip->_channel);
}
