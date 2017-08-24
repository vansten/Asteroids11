#include "MenuScene.h"

#include "Gameplay/ShootToStartActor.h"

bool MenuScene::Initialize(ResourceManager& resourceManager)
{
	_camera = NewObject(Camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), CameraSettings(60.0f, 1024.0f, 768.0f, 0.1f, 100.0f));
	_camera->Initialize(resourceManager);

	UIActor* shootToStartActor = SpawnUIActor<UIActor>(glm::vec2(512.0f - 120.0f, 384.0f - 18.0f));
	shootToStartActor->SetText("Shoot to start!");

	SpawnActor<ShootToStartActor>();

	return true;
}
