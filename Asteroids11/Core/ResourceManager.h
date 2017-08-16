#pragma once

#include <vector>

#include "Rendering/Shader.h"
#include "Audio/AudioClip.h"

class ResourceManager
{
protected:
	std::vector<Shader*> _shaders;
	std::vector<AudioClip*> _audioClips;

protected:
	template<typename T>
	void ClearResources(std::vector<T*> resources)
	{
		auto& it = resources.begin();
		for(; it != resources.end(); ++it)
		{
			Memory::GetInstance()->Deallocate<T>(*it);
			(*it) = nullptr;
		}
		resources.clear();
	}

public:
	void Shutdown();

	Shader* GetShader(const std::string& vertexShader, const std::string& fragmentShader);
	AudioClip* GetAudioClip(const std::string& wavFileName);
};

