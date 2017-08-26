#include "GameplayScene.h"

#include "Core/Input.h"
#include "Core/ResourceManager.h"
#include "Gameplay/AsteroidSpawner.h"
#include "Gameplay/Ship.h"
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Cube.h"
#include "Rendering/Graphics.h"
#include "Rendering/Light.h"

bool GameplayScene::Initialize(ResourceManager& resourceManager)
{
	const ScreenSettings& screenSettings = Engine::GetInstance()->GetGraphics().GetScreenSettings();
	_camera = NewObject(Camera, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f), CameraSettings(60.0f, screenSettings.Width, screenSettings.Height, 0.1f, 100.0f));
	_camera->Initialize(resourceManager);

	_directionalLight = NewObject(Light);
	_directionalLight->GetTransform().SetRotation(glm::vec3(glm::pi<float>() * 0.4f, 0.0f, 0.0f));
	_directionalLight->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) * 0.8f);

	_ambientLight = NewObject(Light);
	_ambientLight->SetColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

	Ship* ship = SpawnActor<Ship>();
		
	AsteroidSpawner* spawner = SpawnActor<AsteroidSpawner>();
	spawner->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 5.5f));

	UIActor* textActor = SpawnUIActor<UIActor>(glm::vec2(30.0f, screenSettings.Height - 36.0f));
	textActor->SetText("Asteroids: ");
	textActor->SetFontSize(20);

	//Create asteroids count text actor
	textActor = SpawnUIActor<UIActor>(glm::vec2(130.0f, screenSettings.Height - 36.0f));
	textActor->SetText("0");
	textActor->SetFontSize(20);
	spawner->SetAsteroidsCountText(textActor);

	return true;
}
