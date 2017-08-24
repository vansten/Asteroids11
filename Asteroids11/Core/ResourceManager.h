#pragma once

#include <vector>

#include "Audio/AudioClip.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

#define CUBE "Cube"
#define SPHERE "Sphere"

class ResourceManager
{
protected:
	std::vector<Shader*> _shaders;
	std::vector<AudioClip*> _audioClips;
	std::vector<Mesh*> _meshes;

protected:
	template<typename T>
	void ClearResources(std::vector<T*> resources)
	{
		auto& it = resources.begin();
		auto& end = resources.end();
		for(; it != end; ++it)
		{
			Memory::GetInstance()->Deallocate<T>(*it);
			(*it) = nullptr;
		}
		resources.clear();
	}

	template<typename T>
	void ClearResourcesShutdown(std::vector<T*> resources)
	{
		auto& it = resources.begin();
		auto& end = resources.end();
		for(; it != end; ++it)
		{
			(*it)->Shutdown();
			Memory::GetInstance()->Deallocate<T>(*it);
			(*it) = nullptr;
		}
		resources.clear();
	}

public:
	void Init();
	void Shutdown();

	Shader* GetShader(const std::string& vertexShader, const std::string& fragmentShader);
	AudioClip* GetAudioClip(const std::string& wavFileName);
	Mesh* GetMesh(const std::string& meshName);
};

