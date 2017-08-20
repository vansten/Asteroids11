#pragma once

#include <string>

#include "FMOD\fmod.hpp"

class AudioClip
{
	friend class Audio;
	friend class AudioSource;

protected:
	std::string _wavFileName;
	FMOD::Sound* _sound;

public:
	AudioClip(const std::string& wavFileName);
	~AudioClip();

	bool Initialize();

	inline const std::string& GetWAVFileName() const
	{
		return _wavFileName;
	}

	inline float GetLength() const
	{
		if(!_sound)
		{
			return 0.0f;
		}

		unsigned int lengthMS;
		FMOD_RESULT result = _sound->getLength(&lengthMS, FMOD_TIMEUNIT_MS);

		return result == FMOD_OK ? 0.001f * lengthMS : 0.0f;
	}
};

