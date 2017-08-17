#include "Camera.h"

#include "Audio/AudioClip.h"
#include "Core/Engine.h"
#include "Core/Input.h"
#include "Core/MathHelper.h"
#include "Physics/BoxCollider.h"
#include "Physics/PhysicalBody.h"
#include "Rendering/Cube.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const CameraSettings& settings) : Actor(), _settings(settings)
{
	_transform.SetPosition(position);
	_transform.SetRotation(glm::quat(rotation));
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}
