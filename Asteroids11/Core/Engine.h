#pragma once

#include "Rendering/Graphics.h"
#include "Physics/Physics.h"
#include "Audio/Audio.h"
#include "ResourceManager.h"
#include "Scene.h"

class Engine
{
protected:
	static Engine* _instance;

public:
	inline static Engine* GetInstance()
	{
		if(_instance == nullptr)
		{
			_instance = NewObject(Engine);
		}

		return _instance;
	}

protected:
	Graphics* _graphics;
	Physics* _physics;
	Audio* _audio;
	Scene* _currentScene;
	ResourceManager _resourceManager;

protected:
	bool ShouldQuit() const;

public:
	Engine();
	~Engine();

	bool Initialize();
	void Run();
	void Shutdown();

	inline Graphics& GetGraphics()
	{
		return *_graphics;
	}

	inline Physics& GetPhysics()
	{
		return *_physics;
	}

	inline Audio& GetAudio()
	{
		return *_audio;
	}

	inline ResourceManager& GetResourceManager()
	{
		return _resourceManager;
	}

	inline Scene* GetCurrentScene()
	{
		return _currentScene;
	}
};