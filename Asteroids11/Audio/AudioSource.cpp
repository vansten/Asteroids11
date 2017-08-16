#include "AudioSource.h"

#include "AudioClip.h"
#include "Core/Engine.h"
#include "Gameplay/Actor.h"

AudioSource::AudioSource(AudioClip* clip) : _clip(clip), _channel(nullptr)
{
	clip->_sound->setMode(FMOD_3D);
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

	FMOD_VECTOR pos;
	pos.x = _owner->GetTransform().GetPosition().x;
	pos.y = _owner->GetTransform().GetPosition().y;
	pos.z = _owner->GetTransform().GetPosition().z;
	FMOD_VECTOR vel;
	_channel->set3DAttributes(&pos, &vel);
	Engine::GetInstance()->GetAudio().PlaySound(this);
	_channel->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

void AudioSource::Stop()
{
	if(_channel != nullptr)
	{
		_channel->stop();
	}
}
