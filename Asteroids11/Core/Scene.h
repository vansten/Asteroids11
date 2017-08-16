#pragma once

#include <vector>

#include "Gameplay/Actor.h"
#include "Gameplay/Camera.h"

class Scene
{
protected:
	std::vector<Actor*> _actors;
	Camera* _camera;

public:
	Scene();
	~Scene();

	bool Initialize(class ResourceManager& resourceManager);
	void Shutdown();

	void Update(GLfloat deltaTime);
	void PreSimulate();
	void PostSimulate();
	void Render(class Graphics* graphics);

	Actor* SpawnActor(const Transform& transform);
	Actor* SpawnActor(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
};

