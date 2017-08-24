#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "Actor.h"

struct CameraSettings
{
protected:
	float _fov;
	float _aspectRatio;
	float _width;
	float _height;
	float _near;
	float _far;

public:
	inline CameraSettings(float fov, float width, float height, float near, float far) : _fov(fov), _width(width), _height(height), _aspectRatio(width / height), _near(near), _far(far) { }
	inline CameraSettings(const CameraSettings& other) : _fov(other._fov), _width(other._width), _height(other._height), _aspectRatio(other._aspectRatio), _near(other._near), _far(other._far) { }

	inline float GetFov() const { return _fov; }
	inline float GetWidth() const { return _width; }
	inline float GetHeight() const { return _height; }
	inline float GetAspectRatio() const { return _aspectRatio; }
	inline float GetNear() const { return _near; }
	inline float GetFar() const { return _far; }
};

class Camera : public Actor
{
protected:
	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;
	glm::mat4 _uiProjectionMatrix;

	CameraSettings _settings;

protected:
	inline void UpdateViewMatrix()
	{
		_viewMatrix = glm::lookAt(_transform.GetPosition(), _transform.GetPosition() + _transform.GetForward(), _transform.GetUp());
	}

	inline void UpdateProjectionMatrix()
	{
		_projectionMatrix = glm::perspective(glm::radians(_settings.GetFov()), _settings.GetAspectRatio(), _settings.GetNear(), _settings.GetFar());
		_uiProjectionMatrix = glm::ortho(0.0f, _settings.GetWidth(), 0.0f, _settings.GetHeight());
	}

public:
	Camera(const glm::vec3& position, const glm::vec3& rotation, const CameraSettings& settings);
	virtual ~Camera();

	virtual void Update(float deltaTime) override;
	virtual void OnModelMatrixUpdated() override
	{
		UpdateViewMatrix();
	}

	inline const glm::mat4& GetViewMatrix() const
	{
		return _viewMatrix;
	}

	inline const glm::mat4& GetProjectionMatrix() const
	{
		return _projectionMatrix;
	}

	inline const glm::mat4& GetUIProjectionMatrix() const
	{
		return _uiProjectionMatrix;
	}

	bool IsPointVisible(const glm::vec3& point, const glm::mat4& modelMatrix) const;
};

