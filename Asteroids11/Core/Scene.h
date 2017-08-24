#pragma once

#include <vector>

#include "Engine.h"
#include "Gameplay/Actor.h"
#include "Gameplay/Camera.h"
#include "Gameplay/UIActor.h"

class Light;

class Scene
{
protected:
	std::vector<Actor*> _actors;
	std::vector<Actor*> _pendingActors;
	std::vector<UIActor*> _uiActors;
	std::vector<UIActor*> _pendingUIActors;
	Camera* _camera;
	Light* _directionalLight;
	Light* _ambientLight;

public:
	virtual bool Initialize(class ResourceManager& resourceManager) = 0;
	void Shutdown();

	void Update(GLfloat deltaTime);
	void PreSimulate();
	void PostSimulate();
	void Render(class Graphics* graphics);

	inline Camera* GetCamera() const
	{
		return _camera;
	}

	template<typename T>
	T* SpawnActor(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		T* actor = NewObject(T);
		actor->GetTransform().SetPosition(position);
		actor->GetTransform().SetRotation(rotation);
		actor->GetTransform().SetScale(scale);
		actor->Initialize(Engine::GetInstance()->GetResourceManager());
		_pendingActors.push_back(actor);
		return actor;
	}

	template<typename T>
	T* SpawnUIActor(const glm::vec2& screenPosition)
	{
		T* uiActor = NewObject(T);
		uiActor->GetTransform().SetScreenPosition(screenPosition);
		uiActor->Initialize(Engine::GetInstance()->GetResourceManager());
		_pendingUIActors.push_back(uiActor);
		return uiActor;
	}
};

