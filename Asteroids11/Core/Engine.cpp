#include "Engine.h"

#include "Input.h"
#include "MathHelper.h"
#include "Gameplay/Scenes/GameplayScene.h"
#include "Gameplay/Scenes/MenuScene.h"

Engine* Engine::_instance = nullptr;

bool Engine::ShouldQuit() const
{
	return Input::GetKey(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(_graphics->GetWindow()) != 0;
}

Engine::Engine() : _currentScene(nullptr), _graphics(nullptr), _physics(nullptr), _audio(nullptr), _nextScene(nullptr)
{
	if(_instance != nullptr)
	{
		_instance->Shutdown();
		Memory::GetInstance()->Deallocate<Engine>(_instance);
		_instance = nullptr;
	}

	_instance = this;
}

Engine::~Engine()
{

}

bool Engine::Initialize()
{
	MathHelper::InitRand();

	_graphics = NewObject(Graphics);
	if(!_graphics->Initialize())
	{
		printf("Cannot initialize graphics\n");
		return false;
	}

	_resourceManager.Init();

	Input::Initialize(_graphics->GetWindow());

	_physics = NewObject(Physics);
	if(!_physics->Initialize())
	{
		printf("Cannot initialize physics\n");
		return false;
	}

	_audio = NewObject(Audio);
	if(!_audio->Initialize())
	{
		printf("Cannot initialize audio\n");
		return false;
	}

	_currentScene = NewObject(MenuScene);
	if(!_currentScene->Initialize(_resourceManager))
	{
		printf("Cannot initialize scene\n");
		return false;
	}

	PhysicsHelper::SetPhysics(_physics);

	return true;
}

void Engine::Run()
{
	float fixedTimeStep = 1.0f / 60.0f;
	double frameTime = 1.0 / 60.0;
	double prevTime = glfwGetTime();
	double timer = 0.0f;
	while(!ShouldQuit())
	{
		if(_nextScene)
		{
			_physics->Reload();
			_currentScene->Shutdown();
			Memory::GetInstance()->Deallocate<Scene>(_currentScene);
			_currentScene = _nextScene;
			_currentScene->Initialize(_resourceManager);
			_nextScene = nullptr;
		}

		_currentScene->PreSimulate();
		_physics->Update(fixedTimeStep);
		_currentScene->PostSimulate();
		_physics->Resolve();

		_currentScene->Update((GLfloat)frameTime);

		_audio->Update((float)frameTime);

		_graphics->Clear();

		_currentScene->Render(_graphics);

		_graphics->Present();

		glfwPollEvents();

		frameTime = glfwGetTime() - prevTime;
		prevTime += frameTime;
		timer += frameTime;
		if(timer >= 1.0f)
		{
			timer = 0.0f;
			printf("Frame time: %lf, fps: %lf\n", frameTime, 1.0f / frameTime);
			Memory::GetInstance()->PrintStatistics();
		}

		if(Input::GetKey(GLFW_KEY_F8))
		{
			Memory::GetInstance()->PrintActualStatus();
		}

		if(Input::GetKey(GLFW_KEY_F9))
		{
			PxU32 actorsCount = _physics->GetScene()->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
			printf("Physx actors: %d\n", actorsCount);
		}
	}
}

void Engine::Shutdown()
{
	PhysicsHelper::SetPhysics(nullptr);

	if(_audio)
	{
		_audio->Shutdown();
		Memory::GetInstance()->Deallocate<Audio>(_audio);
		_audio = nullptr;
	}

	if(_physics)
	{
		_physics->Shutdown();
		Memory::GetInstance()->Deallocate<Physics>(_physics);
		_physics = nullptr;
	}

	if(_graphics)
	{
		_graphics->Shutdown();
		Memory::GetInstance()->Deallocate<Graphics>(_graphics);
		_graphics = nullptr;
	}

	if(_currentScene)
	{
		_currentScene->Shutdown();
		Memory::GetInstance()->Deallocate<Scene>(_currentScene);
		_currentScene = nullptr;
	}

	if(_nextScene)
	{
		_nextScene->Shutdown();
		Memory::GetInstance()->Deallocate<Scene>(_nextScene);
		_nextScene = nullptr;
	}

	Input::Shutdown();

	_resourceManager.Shutdown();
}

void Engine::LoadGameplayScene()
{
	_nextScene = NewObject(GameplayScene);
}

void Engine::LoadMenuScene()
{
	_nextScene = NewObject(MenuScene);
}
