#include "Engine.h"

#include "Input.h"
#include "Helper.h"
#include "MathHelper.h"
#include "Gameplay/Scenes/GameplayScene.h"
#include "Gameplay/Scenes/MenuScene.h"

Engine* Engine::_instance = nullptr;

#define SIZE_IN_BYTES(x) x
#define SIZE_IN_KILOBYTES(x) x / 1024.0f
#define SIZE_IN_MEGABYTES(x) (SIZE_IN_KILOBYTES(x)) / 1024.0f

bool Engine::ShouldQuit() const
{
	return Input::GetKey(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(_graphics->GetWindow()) != 0;
}

void Engine::FillStatisticsText(float frameTime)
{
	float fps = 1.0f / frameTime;
	PxU32 physxActorsCount = _physics->GetScene()->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	unsigned int memoryAllocated = Memory::GetInstance()->GetMemoryAllocated();
	unsigned int objectsCount = Memory::GetInstance()->GetObjectsCount();

	if(_statisticsActor)
	{
		std::string text = "FPS: " + ToString(fps, 3) + ", frameTime: " + ToString(frameTime, 3) + "ms\n";
		text += "PhysxActors: " + ToString(physxActorsCount, 0) + "\n";
		text += "Memory, objects: " + ToString(objectsCount, 0) + "\n";
		text += "Memory size: " + ToString(SIZE_IN_BYTES(memoryAllocated), 0) + " B, ";
		text += ToString(SIZE_IN_KILOBYTES(memoryAllocated), 3) + " KB, ";
		text += ToString(SIZE_IN_MEGABYTES(memoryAllocated), 3) + " MB";
		_statisticsActor->SetText(text);
	}
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

	const ScreenSettings& screenSettings = _graphics->GetScreenSettings();
	_statisticsActor = NewObject(UIActor);
	_statisticsActor->GetTransform().SetScreenPosition(glm::vec2(screenSettings.Width - 300.0f, screenSettings.Height - 36.0f));
	_statisticsActor->Initialize(_resourceManager);
	_statisticsActor->SetFontSize(12);

	return true;
}

void Engine::Run()
{
	float fixedTimeStep = 1.0f / 60.0f;
	double frameTime = 1.0 / 60.0;
	double prevTime = glfwGetTime();
	double timer = 0.0f;

	bool wasF8Pressed = Input::GetKey(GLFW_KEY_F8);
	bool isF8Pressed = Input::GetKey(GLFW_KEY_F8);
	bool drawStats = false;

	FillStatisticsText((float)frameTime);

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

		isF8Pressed = Input::GetKey(GLFW_KEY_F8);
		if(!wasF8Pressed && isF8Pressed)
		{
			drawStats = !drawStats;
		}
		wasF8Pressed = isF8Pressed;

		_currentScene->PreSimulate();
		_physics->Update(fixedTimeStep);
		_currentScene->PostSimulate();
		_physics->Resolve();

		_currentScene->Update((GLfloat)frameTime);

		_audio->Update((float)frameTime);

		_graphics->Clear();

		_currentScene->Render(_graphics);
		
		if(drawStats)
		{
			_graphics->BeginUI();
			_statisticsActor->Render(*_currentScene->GetCamera(), _graphics);
			_graphics->EndUI();
		}

		_graphics->Present();

		glfwPollEvents();

		frameTime = glfwGetTime() - prevTime;
		prevTime += frameTime;
		timer += frameTime;

		if(timer >= 1.0f)
		{
			timer = 0.0f;
			FillStatisticsText((float)frameTime);
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

	if(_statisticsActor)
	{
		_statisticsActor->Shutdown();
		Memory::GetInstance()->Deallocate<UIActor>(_statisticsActor);
		_statisticsActor = nullptr;
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
