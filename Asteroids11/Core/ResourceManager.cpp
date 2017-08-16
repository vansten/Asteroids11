#include "ResourceManager.h"

#include "Memory.h"

void ResourceManager::Shutdown()
{
	ClearResources(_shaders);
	ClearResources(_audioClips);
}

Shader* ResourceManager::GetShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	std::string key = vertexShader + "_" + fragmentShader;
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
	std::vector<AudioClip*>::iterator it = _audioClips.begin();
	for(; it != _audioClips.end(); ++it)
	{
		if(wavFileName.compare((*it)->GetWAVFileName()))
		{
			return (*it);
		}
	}

	AudioClip* ac = NewObject(AudioClip, "Assets/" + wavFileName);
	ac->Initialize();
	_audioClips.push_back(ac);
	return ac;
}
