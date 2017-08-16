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
};

