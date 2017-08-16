#pragma once

#include "FMOD\fmod.hpp"

class AudioClip;
class Actor;

class AudioSource
{
	friend class Audio;

protected:
	AudioClip* _clip;
	FMOD::Channel* _channel;
	Actor* _owner;

public:
	AudioSource(AudioClip* clip);
	~AudioSource();

	void Play(bool loop = false);
	void Stop();

	inline void SetOwner(Actor* a)
	{
		_owner = a;
	}

	inline bool IsPlaying()
	{
		if(_channel == nullptr)
		{
			return false;
		}

		bool isPlaying;
		_channel->isPlaying(&isPlaying);
		return isPlaying;
	}
};

