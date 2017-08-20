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
	float _volume;

public:
	AudioSource(AudioClip* clip);
	~AudioSource();

	void Play(bool loop = false);
	void Stop();

	float GetClipLength() const;

	inline void SetOwner(Actor* a)
	{
		_owner = a;
	}

	inline void SetVolume(float volume)
	{
		_volume = volume;
		if(_channel)
		{
			_channel->setVolume(_volume);
		}
	}

	inline float GetVolume() const
	{
		return _volume;
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

