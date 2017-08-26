#pragma once

#include "Rendering/Graphics.h"
#include "Physics/Physics.h"
#include "Audio/Audio.h"
#include "ResourceManager.h"

class Scene;
class UIActor;

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
	Scene* _nextScene;

	UIActor* _statisticsActor;

protected:
	bool ShouldQuit() const;

	void FillStatisticsText(float frameTime);

public:
	Engine();
	~Engine();

	bool Initialize();
	void Run();
	void Shutdown();

	void LoadGameplayScene();
	void LoadMenuScene();

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

	inline Scene* GetScene()
	{
		//Return _nextScene if it's not null cause in this case it's the _nextScene where should new actors be spawned
		return _nextScene ? _nextScene : _currentScene;
	}
};