#include "MenuScene.h"

#include "Gameplay/ShootToStartActor.h"

bool MenuScene::Initialize(ResourceManager& resourceManager)
{
	const ScreenSettings& screenSettings = Engine::GetInstance()->GetGraphics().GetScreenSettings();
	_camera = NewObject(Camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), CameraSettings(60.0f, screenSettings.Width, screenSettings.Height, 0.1f, 100.0f));
	_camera->Initialize(resourceManager);

	UIActor* shootToStartActor = SpawnUIActor<UIActor>(glm::vec2(screenSettings.Width * 0.5f - 120.0f, screenSettings.Height * 0.5f - 18.0f));
	shootToStartActor->SetText("Shoot to start!");

	SpawnActor<ShootToStartActor>();

	return true;
}
