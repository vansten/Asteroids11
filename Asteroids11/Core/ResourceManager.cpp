#include "ResourceManager.h"

#include "Memory.h"
#include "Rendering/Cube.h"

void ResourceManager::Init()
{
	Shader* basicShader = GetShader("Shaders/basicVS.glsl", "Shaders/basicFS.glsl");
	Mesh* m = NewObject(Cube, 1.0f);
	m->Initialize();
	_meshes.push_back(m);
}

void ResourceManager::Shutdown()
{
	ClearResourcesShutdown(_meshes);
	ClearResources(_shaders);
	ClearResources(_audioClips);
}

Shader* ResourceManager::GetShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	std::string key = "Assets/" + vertexShader + "_Assets/" + fragmentShader;
	std::vector<Shader*>::iterator it = _shaders.begin();

	for(; it != _shaders.end(); ++it)
	{
		std::string shaderKey = (*it)->GetVertexShaderFilename() + "_" + (*it)->GetFragmentShaderFilename();
		if(key.compare(shaderKey) == 0)
		{
			return (*it);
		}
	}

	Shader* s = NewObject(Shader, "Assets/" + vertexShader, "Assets/" + fragmentShader);
	s->Initialize();
	_shaders.push_back(s);
	return s;
}

AudioClip* ResourceManager::GetAudioClip(const std::string& wavFileName)
{
	std::string compareKey = "Assets/" + wavFileName;
	std::vector<AudioClip*>::iterator it = _audioClips.begin();
	for(; it != _audioClips.end(); ++it)
	{
		if(wavFileName.compare((*it)->GetWAVFileName()) == 0)
		{
			return (*it);
		}
	}

	AudioClip* ac = NewObject(AudioClip, compareKey);
	ac->Initialize();
	_audioClips.push_back(ac);
	return ac;
}

Mesh* ResourceManager::GetMesh(const std::string& meshName)
{
	std::vector<Mesh*>::iterator it = _meshes.begin();
	for(; it != _meshes.end(); ++it)
	{
		if(meshName.compare((*it)->GetName()) == 0)
		{
			return (*it);
		}
	}

	//Do not create new meshes (since the framework supports only cube mesh and they're added to vector during the initialization)
	return nullptr;
}
