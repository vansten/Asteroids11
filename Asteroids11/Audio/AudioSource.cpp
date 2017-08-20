#include "AudioSource.h"

#include "AudioClip.h"
#include "Core/Engine.h"
#include "Gameplay/Actor.h"

AudioSource::AudioSource(AudioClip* clip) : _clip(clip), _channel(nullptr), _volume(1.0f)
{
	clip->_sound->setMode(FMOD_2D);
}


AudioSource::~AudioSource()
{

}

void AudioSource::Play(bool loop)
{
	if(IsPlaying())
	{
		Stop();
	}

	Engine::GetInstance()->GetAudio().PlaySound(this);
	_channel->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	_channel->setVolume(_volume);
}

void AudioSource::Stop()
{
	if(_channel != nullptr)
	{
		_channel->stop();
	}
}

float AudioSource::GetClipLength() const
{
	return _clip ? _clip->GetLength() : 0.0f;
}
