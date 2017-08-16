#pragma once

#include "FMOD\fmod.hpp"
#include "FMOD\fmod_errors.h"

#if _DEBUG
#pragma comment(lib, "fmodL64_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

class Audio
{
protected:
	FMOD::System* _fmod;

public:
	Audio();
	~Audio();

	bool Initialize();
	void Shutdown();

	void Update(float deltaTime);

	void PlaySound(class AudioSource* clip);

	inline FMOD::System& GetFMODSystem() const
	{
		return *_fmod;
	}
};

